import os

from flask import Flask
from flask_admin import Admin

from database.base import db
from database.posts.models import Posts
from database.users.models import Users
from views.admin import UsersAdmin, PostsAdmin


app = Flask(__name__)
app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY')


admin = Admin(app)
admin.add_view(UsersAdmin(Users, db))
admin.add_view(PostsAdmin(Posts, db))
