from flask import render_template, request, session, redirect, url_for, flash
from flask.views import MethodView
from werkzeug.security import check_password_hash

from database.posts.models import Posts
from database.users.models import Users
from utils import pagination
from utils.tasks import create_report
from utils.auth import login_required


def add_to_session(user: Users, remember_me=False):
    for key, value in user.__dict__.items():
        if key not in ['_sa_instance_state', 'password_hash']:
            session[key] = value
    session.permanent = remember_me
    session.modified = True


class StartPage(MethodView):

    def get(self):
        return render_template("index.html")


class UserView(MethodView):

    @login_required
    def get(self, page=1):
        search_text = request.args.get('text')
        current_user = Users.get_by_id(session['id'])
        posts_query = current_user.get_posts(search_text)
        page = pagination.Paginator(posts_query, page, 50)
        return render_template("user_page.html", user_name=session['username'],
                               page=page
                               )


class OtherUser(MethodView):

    @login_required
    def get(self, id):
        current_user = Users.get_by_id(session['id'])
        other_user = Users.get_by_id(id)
        exists = current_user in other_user.get_subscribers()
        return render_template("other_user_profile.html",
                               user_name=session['username'],
                               other_user=other_user, exists=exists)


class Login(MethodView):

    def get(self):
        return render_template("login.html")

    def post(self):
        email = request.form['email'].lower()
        password = request.form['password']
        remember_me = bool(request.form.get('remember'))
        user = Users.get_by_email(email)
        if not user or not check_password_hash(user.password, password):
            return render_template("login.html",
                                   error="Wrong email or password")
        add_to_session(user, remember_me)
        return redirect(url_for('get_user_page'))


class Logout(MethodView):

    @login_required
    def get(self):
        session.pop('id')
        session.pop('username')
        session.pop('email')
        return redirect(url_for('get_start_page'))


class SignUp(MethodView):

    def get(self):
        return render_template("sign_up.html")

    def post(self):
        email = request.form['email'].lower()
        username = request.form['username']
        password = request.form['password']
        if Users.get_by_email(email):
            return render_template("sign_up.html", error="Such email exists")
        user = Users.user_sign_up(username, password, email)
        add_to_session(user)
        return redirect(url_for("get_user_page"))


class Create(MethodView):

    @login_required
    def get(self):
        return render_template("create.html", user_name=session['username'])

    @login_required
    def post(self):
        title = request.form['title']
        post = request.form['post']
        post_type = request.form['post_type']
        Posts.create_post(title, post, post_type, session["id"])
        flash("Пост успешно создан", 'post_create')
        create_report.delay(session['id'])
        # create_report(session['id'])
        return render_template("create.html", user_name=session['username'])


class Subscribe(MethodView):

    @login_required
    def get(self):
        user = Users.get_by_id(session['id'])
        respondents = user.get_respondents()
        return render_template("subscribes.html",
                               user_name=session['username'],
                               respondents=respondents)

    @login_required
    def post(self):
        other_user = int(request.form['other_user'])
        Users.subscribe(session["id"], other_user)
        return redirect(url_for('get_user_page'))


class Unsubscribe(MethodView):

    @login_required
    def post(self):
        other_user = int(request.form['other_user'])
        Users.unsubscribe(session["id"], other_user)
        return redirect(url_for('get_user_page'))
