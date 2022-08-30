import os
from flask import Flask
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

app = Flask(__name__)
app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY')


engine = create_engine(
    f'postgresql+psycopg2://{os.environ.get("USER")}:{os.environ.get("PASSWORD")}@localhost:5432/{os.environ.get("DATABASE")}'
)
Session = sessionmaker(bind=engine)
db = Session()
