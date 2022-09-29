from flask import Flask
from flask_admin import Admin

from utils import config
from database.base import db
from database.posts.models import Posts
from database.users.models import Users
from views.admin import UsersAdmin, PostsAdmin
from views.routes import set_routes


app = Flask(__name__)
app.config['SECRET_KEY'] = config.SECRET_KEY
app.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'


admin = Admin(app)
admin.add_view(UsersAdmin(Users, db))
admin.add_view(PostsAdmin(Posts, db))


if __name__ == '__main__':
    set_routes(app)
    app.run(host="127.0.0.1", port=5000, debug=True)
