from database.base import db
from .models import Posts


def create_post(title: str, post: str, post_type: str, user_id: int) -> None:
    new_post = Posts(
        title=title, post=post, post_type=post_type, user_id=user_id
    )
    db.add(new_post)
    db.commit()


def get_own_posts(id: int) -> list:
    user_posts = db.query(Posts).filter(Posts.user_id == id)
    return user_posts.all()


def delete_post(post_id: int) -> None:
    post = db.query(Posts).get(post_id)
    db.delete(post)
    db.commit()

