from flask import session, redirect, url_for, flash


def login_required(function):
    def wrapper(*args, **kwargs):
        if not session.get('id'):
            flash("Пожалуйста, авторизуйтесь", 'login_message')
            return redirect(url_for('login'))
        return function(*args, **kwargs)
    return wrapper
