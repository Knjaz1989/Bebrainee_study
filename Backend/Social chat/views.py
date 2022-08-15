from datetime import datetime
from flask import render_template, request, session, redirect, flash, url_for
from flask.views import MethodView
from werkzeug.security import check_password_hash, generate_password_hash
from application import app
from auth import check_user, login_required
from database import User, Post, db


class StartPage(MethodView):

    @check_user
    def get(self):
        return render_template("index.html")


class UserView(MethodView):

    @login_required
    def get(self):
        current_user = User.get_user(self, session['id'])
        respondent_posts = current_user.get_respondent_posts()
        other_posts = current_user.get_other_posts()
        return render_template("user_page.html", user_name=session['username'],
                               respondent_posts=respondent_posts,
                               posts=other_posts)


class OtherUser(MethodView):

    @login_required
    def get(self, id):
        current_user = User.get_user(self, session['id'])
        other_user = User.get_user(self, request.view_args['id'])
        if current_user in other_user.subscribers:
            exists = True
        else:
            exists = False
        return render_template("other_user_profile.html", user_name=session['username'],
                               other_user=other_user, exists=exists)


class Login(MethodView):

    @check_user
    def get(self):
        return render_template("login.html")

    @check_user
    def post(self):
        email = request.form.get('email').lower()
        password = request.form.get('password')
        remember_me = bool(request.form.get('remember'))
        user = db.query(User).filter(User.email == email).first()
        if user and check_password_hash(user.password_hash, password):
            for key, value in user.__dict__.items():
                if key not in ['_sa_instance_state', 'password_hash']:
                    session[key] = value
            session.permanent = remember_me
            session.modified = True
            return redirect(url_for('get_user_page'))
        return render_template("login.html", error="Wrong email or password")


class Logout(MethodView):

    @login_required
    def get(self):
        session.pop('id')
        session.pop('username')
        session.pop('email')
        return redirect(url_for('get_start_page'))


class SignUp(MethodView):

    @check_user
    def get(self):
        return render_template("sign_up.html")

    @check_user
    def post(self):
        email = request.form.get('email').lower()
        exists_email = db.query(User).filter_by(email=email).first()
        if exists_email:
            return render_template("sign_up.html", error="Such email exists")
        password = request.form.get('password')
        username = request.form.get('name')
        new_user = User(username=username, email=email, password_hash=generate_password_hash(password))
        db.add(new_user)
        db.commit()
        return redirect(url_for("login"))


class Create(MethodView):

    @login_required
    def get(self):
        return render_template("create.html", user_name=session['username'])

    @login_required
    def post(self):
        title = request.form.get('title')
        post = request.form.get('post')
        post_type = request.form.get('post_type')
        new_post = Post(
            title=title, post=post, post_type=post_type, user_id=session['id'],
            created_at=datetime.now()
        )
        db.add(new_post)
        db.commit()
        flash("Пост успешно создан", 'post_create')
        return render_template("create.html", user_name=session['username'])


class Subscribe(MethodView):

    @login_required
    def get(self):
        user = User.get_user(self, session['id'])
        respondents = user.respondents
        return render_template("subscribes.html", user_name=session['username'], respondents=respondents)

    @login_required
    def post(self):
        user = User.get_user(self, session['id'])
        respondent = User.get_user(self, int(request.form.get('other_user')))
        user.respondents.append(respondent)
        db.commit()
        return redirect(url_for('get_user_page'))


class Unsubscribe(MethodView):

    @login_required
    def post(self):
        user = User.get_user(self, session['id'])
        respondent = User.get_user(self, int(request.form.get('other_user')))
        user.respondents.remove(respondent)
        db.commit()
        return redirect(url_for('subscribe'))


app.add_url_rule('/', view_func=StartPage.as_view('get_start_page'), methods=["GET"])
app.add_url_rule('/user', view_func=UserView.as_view('get_user_page'), methods=["GET"])
app.add_url_rule('/user/<id>', view_func=OtherUser.as_view('other_user'), methods=["GET"])
app.add_url_rule('/login', view_func=Login.as_view('login'), methods=["GET", "POST"])
app.add_url_rule('/logout', view_func=Logout.as_view('logout'), methods=["GET"])
app.add_url_rule('/signup', view_func=SignUp.as_view('sign_up'), methods=["GET", "POST"])
app.add_url_rule('/create', view_func=Create.as_view('create_post'), methods=["GET", "POST"])
app.add_url_rule('/subscribe', view_func=Subscribe.as_view('subscribe'), methods=["GET", "POST", 'DELETE'])
app.add_url_rule('/unsubscribe', view_func=Unsubscribe.as_view('unsubscribe'), methods=["POST"])
