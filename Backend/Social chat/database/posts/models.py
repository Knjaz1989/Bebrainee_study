from sqlalchemy import Column, Index, ForeignKey, Integer, String, Enum, \
    DateTime
from sqlalchemy.orm import declarative_base, relationship
from sqlalchemy.sql import func

from database.base import db
from database.users.models import Users

PostBase = declarative_base()


class Posts(PostBase):
    __tablename__ = 'posts'

    id = Column(Integer(), primary_key=True)
    title = Column(String(100), nullable=False)
    post = Column(String(1000), nullable=False)
    post_type = Column(Enum("private", "public", name="post_type"),
                       default='public')
    created_at = Column(DateTime(), default=func.now())
    user_id = Column(ForeignKey(Users.id, ondelete='CASCADE'),
                     nullable=False)
    user = relationship(Users) #, back_populates='posts')
    idx_post_type = Index('idx_post_type', post_type)

    def __str__(self):
        return self.title

    @classmethod
    def create_post(cls, title: str, post: str, post_type: str, user_id: int):
        new_post = cls(
            title=title, post=post, post_type=post_type, user_id=user_id
        )
        db.add(new_post)
        db.commit()
