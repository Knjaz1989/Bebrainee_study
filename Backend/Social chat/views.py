from datetime import timedelta, datetime
from flask import render_template, request, session, redirect, url_for, flash
from flask_login import login_user, current_user, logout_user, login_required, fresh_login_required
from werkzeug.security import check_password_hash, generate_password_hash
from application import app, db
from database import Users, Posts




@app.route("/")
def start_page():
    if current_user.is_authenticated:
        return redirect("/profile")
    return render_template("index.html")


@app.route("/profile")
@login_required
def profile():
    posts = Posts.query.order_by(db.desc(Posts.created_at))
    return render_template("user_page.html", user_name=current_user.username, posts=posts)


@app.route("/login", methods=["GET", "POST"])
def login():
    if current_user.is_authenticated:
        return redirect('/profile')
    if request.method == 'POST':
        email = request.form.get('email').lower()
        password = request.form.get('password')
        remember_ = bool(request.form.get('remember'))
        user = Users.query.filter_by(email=email).first()
        if user and check_password_hash(user.password_hash, password):
            login_user(user, remember=remember_, duration=timedelta(seconds=1))
            return redirect("/")
        return render_template("login.html", error="Wrong email or password")
    return render_template("login.html")


@app.route("/register", methods=["GET", "POST"])
def sign_up():
    if current_user.is_authenticated:
        return redirect('/profile')
    if request.method == 'POST':
        email = request.form.get('email')
        exists_email = Users.query.filter_by(email=email).first()
        if exists_email:
            return render_template("sign_up.html", error="Such email exists")
        password = request.form.get('password')
        username = request.form.get('name')
        new_user = Users(username=username, email=email, password_hash=generate_password_hash(password))
        db.session.add(new_user)
        db.session.commit()
        return render_template("login.html")
    return render_template("sign_up.html")


@app.route('/logout', methods=['GET'])
@login_required
def logout():
    logout_user()
    return redirect("/")


@app.route('/create', methods=['GET', 'POST'])
@login_required
def create():
    if request.method == 'POST':
        title = request.form.get('title')
        post = request.form.get('post')
        post_type = request.form.get('post_type')
        new_post = Posts(title=title, post=post, post_type=post_type, user_id=current_user.id, created_at=datetime.now())
        db.session.add(new_post)
        db.session.commit()
        flash("Пост успешно создан", 'post_create')
    return render_template("create.html", user_name=current_user.username,)


@app.route('/subscribes')
@login_required
def subscribes():

    return render_template("subscribes.html")
