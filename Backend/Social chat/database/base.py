from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker

from utils import config


engine = create_engine(config.PG_URL)
Session = sessionmaker(bind=engine)
db = Session()
