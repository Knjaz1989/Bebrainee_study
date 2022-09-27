import csv

from celery import Celery

from application import app
from database.users.models import Users


app.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'

celery_app = Celery(app.name, broker=app.config['CELERY_BROKER_URL'])


@celery_app.task()
def create_report(id):
    with open(f"reports/report_{id}.csv", "w", encoding='utf-8') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['title', 'post', 'post_type', 'created_at'])
        user_posts = Users.get_by_id(id).posts
        for post in user_posts:
            writer.writerow(
                [post.title, post.post, post.post_type, post.created_at]
            )
