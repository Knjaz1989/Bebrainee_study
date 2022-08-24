from flask import Flask
from flask_debugtoolbar import DebugToolbarExtension
from flask_caching import Cache
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

app = Flask(__name__)
app.config['SECRET_KEY'] = '192b9bdd22ab9ed4d12e236c78afcb9a393ec15f71bbf5dc987d54727823bcbf'
app.config['DEBUG_TB_INTERCEPT_REDIRECTS'] = False

app.debug = True
toolbar = DebugToolbarExtension(app)

cache = Cache(config={'CACHE_TYPE': 'SimpleCache'})
cache.init_app(app)

engine = create_engine('postgresql+psycopg2://db_user:1234@localhost:5432/db_test')
Session = sessionmaker(bind=engine)
db = Session()
