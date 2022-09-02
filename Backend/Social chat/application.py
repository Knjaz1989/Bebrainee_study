import os

from flask import Flask
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker


app = Flask(__name__)
app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY')


engine = create_engine(
    f"postgresql+psycopg2://db_user:1234@localhost:5432/db_test"
)
Session = sessionmaker(bind=engine)
db = Session()
