import csv
from datetime import datetime
from celery import Celery
from flask import request, session, flash
from application import app
from database import User, db, Post


app.config['CELERY_BROKER_URL'] = 'redis://localhost:6379/0'
celery = Celery(app.name, broker=app.config['CELERY_BROKER_URL'])


@celery.task
def create_report(id):
    with open(f"report_{id}.csv", "w", encoding='utf-8') as file:
        writer = csv.writer(file, delimiter=';')
        writer.writerow(['title', 'post', 'post_type', 'created_at'])
        user_posts = db.query(User).get(id).posts
        for post in user_posts:
            writer.writerow([post.title, post.post, post.post_type, post.created_at])


@celery.task
def create_post():
    title = request.form.get('title')
    post = request.form.get('post')
    post_type = request.form.get('post_type')
    new_post = Post(
        title=title, post=post, post_type=post_type, user_id=session['id'],
        created_at=datetime.now()
    )
    db.add(new_post)
    db.commit()
    flash("Пост успешно создан", 'post_create')
    create_report.delay(session['id'])
