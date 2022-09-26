import os

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker


engine = create_engine(os.environ.get('DATABASE_URL'))
Session = sessionmaker(bind=engine)
db = Session()
