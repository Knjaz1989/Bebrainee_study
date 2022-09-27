from flask_admin.contrib.sqla import ModelView
from werkzeug.security import generate_password_hash


class UsersAdmin(ModelView):
    column_list = ('username', 'email', 'respondents', 'subscribers', 'posts')
    form_create_rules = ('username', 'email', 'password', 'subscribers', 'respondents')

    def on_model_change(self, form, model, is_created):
        model.password = generate_password_hash(model.password)
        model.email = model.email.lower()


class PostsAdmin(ModelView):
    form_excluded_columns = ('created_at',)
