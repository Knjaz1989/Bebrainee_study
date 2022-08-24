from flask_admin import Admin
from flask_admin.contrib.sqla import ModelView

from application import app, db
from database import Post, User

admin = Admin(app)

admin.add_view(ModelView(User, db))
admin.add_view(ModelView(Post, db))
