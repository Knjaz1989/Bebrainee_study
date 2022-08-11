from application import db, login_manager


class Users(db.Model):
    __tablename__ = 'users'

    id = db.Column(db.Integer(), primary_key=True)
    username = db.Column(db.String(50), nullable=False)
    email = db.Column(db.String(100), nullable=False, unique=True)
    password_hash = db.Column(db.String(200), nullable=False)

    def is_authenticated(self):
        return True

    def is_active(self):
        return True

    def is_anonymous(self):
        return False

    def get_id(self):
        return str(self.id)



class Posts(db.Model):
    __tablename__ = 'posts'

    id = db.Column(db.Integer(), primary_key=True)
    title = db.Column(db.String(100), nullable=False)
    post = db.Column(db.String(1000), nullable=False)
    post_type = db.Column(db.Enum("private", "public", name="status"))
    created_at = db.Column(db.DateTime())
    user_id = db.Column(db.ForeignKey('users.id', ondelete='CASCADE'))
    pr = db.relationship('Users', backref='posts', uselist=False)


class Subscribes(db.Model):
    __tablename__ = 'subscribes'
    
    id = db.Column(db.Integer(), primary_key=True)
    subscribe_user = db.Column(db.Integer(), nullable=False)
    user_id = db.Column(db.ForeignKey('users.id', ondelete='CASCADE'), nullable=False)


@login_manager.user_loader
def load_user(user_id):
    return Users.query.get(user_id)


db.create_all()
