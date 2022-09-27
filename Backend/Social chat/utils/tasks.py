import csv

from celery import Celery

from database.posts.handlers import get_own_posts


celery_app = Celery('tasks', broker='redis://localhost:6379/0')


@celery_app.task()
def create_report(id):
    with open(f"reports/report_{id}.csv", "w", encoding='utf-8') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['title', 'post', 'post_type', 'created_at'])
        user_posts = get_own_posts(id)
        for post in user_posts:
            writer.writerow(
                [post.title, post.post, post.post_type, post.created_at]
            )
