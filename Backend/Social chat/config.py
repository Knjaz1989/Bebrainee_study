import os

from dotenv import load_dotenv


load_dotenv()


SECRET_KEY = os.environ.get('SECRET_KEY')
PG_URL = os.environ.get('PG_URL')
