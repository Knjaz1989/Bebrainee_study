from application import app
from .views import StartPage, UserView, OtherUser, Login, Logout, SignUp, \
    Create, Subscribe, Unsubscribe


app.add_url_rule('/', view_func=StartPage.as_view('get_start_page'), methods=["GET"])
app.add_url_rule('/user/', view_func=UserView.as_view('get_user_page'), methods=["GET"])
app.add_url_rule('/user/<int:page>/', view_func=UserView.as_view('get_current_page'), methods=["GET"])
app.add_url_rule('/other_user/<int:other_id>/', view_func=OtherUser.as_view('other_user'), methods=["GET"])
app.add_url_rule('/login/', view_func=Login.as_view('login'), methods=["GET", "POST"])
app.add_url_rule('/logout/', view_func=Logout.as_view('logout'), methods=["GET"])
app.add_url_rule('/signup/', view_func=SignUp.as_view('sign_up'), methods=["GET", "POST"])
app.add_url_rule('/create/', view_func=Create.as_view('create_post'), methods=["GET", "POST"])
app.add_url_rule('/subscribe/', view_func=Subscribe.as_view('subscribe'), methods=["GET", "POST", 'DELETE'])
app.add_url_rule('/unsubscribe/', view_func=Unsubscribe.as_view('unsubscribe'), methods=["POST"])
