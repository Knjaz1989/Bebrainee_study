from flask import render_template, request, session, redirect, url_for, flash
from flask.views import MethodView
from werkzeug.security import check_password_hash
from application import app
from database.models import Users, Posts
from utils import pagination
from utils.tasks import create_report
from utils.auth import login_required


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
        return render_template("other_user_profile.html", user_name=session['username'],
                               other_user=other_user, exists=exists)


class Login(MethodView):

    def get(self):
        return render_template("login.html")

    def post(self):
        email = request.form.get('email').lower()
        password = request.form.get('password')
        remember_me = bool(request.form.get('remember'))
        user = Users.get_by_email(email)
        if user and check_password_hash(user.password, password):
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

    def get(self):
        return render_template("sign_up.html")

    def post(self):
        email = request.form.get('email', '').lower()
        if Users.get_by_email(email):
            return render_template("sign_up.html", error="Such email exists")
        Users.user_sign_up(request, email)
        return redirect(url_for("login"))


class Create(MethodView):

    @login_required
    def get(self):
        return render_template("create.html", user_name=session['username'])

    @login_required
    def post(self):
        Posts.create_post(request, session["id"])
        flash("Пост успешно создан", 'post_create')
        create_report.delay(session['id'])
        # create_report(session['id'])
        return render_template("create.html", user_name=session['username'])


class Subscribe(MethodView):

    @login_required
    def get(self):
        user = Users.get_by_id(session['id'])
        respondents = user.get_respondents()
        return render_template("subscribes.html", user_name=session['username'], respondents=respondents)

    @login_required
    def post(self):
        Users.subscribe(request, session["id"])
        return redirect(url_for('get_user_page'))


class Unsubscribe(MethodView):

    @login_required
    def post(self):
        Users.unsubscribe(request, session["id"])
        return redirect(url_for('get_user_page'))


app.add_url_rule('/', view_func=StartPage.as_view('get_start_page'), methods=["GET"])
app.add_url_rule('/user/', view_func=UserView.as_view('get_user_page'), methods=["GET"])
app.add_url_rule('/user/<int:page>/', view_func=UserView.as_view('get_current_page'), methods=["GET"])
app.add_url_rule('/other_user/<int:id>/', view_func=OtherUser.as_view('other_user'), methods=["GET"])
app.add_url_rule('/login/', view_func=Login.as_view('login'), methods=["GET", "POST"])
app.add_url_rule('/logout/', view_func=Logout.as_view('logout'), methods=["GET"])
app.add_url_rule('/signup/', view_func=SignUp.as_view('sign_up'), methods=["GET", "POST"])
app.add_url_rule('/create/', view_func=Create.as_view('create_post'), methods=["GET", "POST"])
app.add_url_rule('/subscribe/', view_func=Subscribe.as_view('subscribe'), methods=["GET", "POST", 'DELETE'])
app.add_url_rule('/unsubscribe/', view_func=Unsubscribe.as_view('unsubscribe'), methods=["POST"])
