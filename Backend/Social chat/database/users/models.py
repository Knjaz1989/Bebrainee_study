from sqlalchemy import Column, Integer, String, ForeignKey, Table, \
    desc, and_, or_
from sqlalchemy.orm import declarative_base, relationship


UserBase = declarative_base()


users_subscribes = Table('users_subscribes', UserBase.metadata,
                         Column('respondent_id', Integer,
                                ForeignKey('users.id')),
                         Column('subscriber_id', Integer,
                                ForeignKey('users.id'))
                         )


class Users(UserBase):
    __tablename__ = 'users'

    id = Column(Integer, primary_key=True)
    username = Column(String(50), nullable=False)
    email = Column(String(100), nullable=False, unique=True)
    password = Column(String(200), nullable=False)
    respondents = relationship(
        'Users', secondary=users_subscribes,
        primaryjoin=users_subscribes.c.subscriber_id == id,
        secondaryjoin=users_subscribes.c.respondent_id == id,
        backref="subscribers"
    )

    def __str__(self):
        return self.email
