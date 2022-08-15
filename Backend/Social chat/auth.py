from functools import wraps
from flask import session, redirect, url_for, flash


def login_required(function):
    wraps(function)

    def wrapper(*args, **kwargs):
        if not session.get('id'):
            flash("Пожалуйста, авторизуйтесь", 'login_message')
            return redirect(url_for('login'))
        return function(*args, *kwargs)
    return wrapper


def check_user(function):
    wraps(function)

    def wrapper(*args, **kwargs):
        if session.get('id'):
            return redirect(url_for('get_user_page'))
        return function(*args, **kwargs)
    return wrapper
