import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    stocks = db.execute("SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0",
                        user_id=session["user_id"])

    cash = db.execute("SELECT cash FROM users WHERE id = :user_id",
                      user_id=session["user_id"])[0]["cash"]

    total_value = cash
    grand_total = cash

    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["value"] = stock["price"] * stock["total_shares"]
        total_value += stock["value"]
        grand_total += stock["value"]

    formatted_stocks = []
    for stock in stocks:
        formatted_stock = stock.copy()
        formatted_stock["price"] = usd(stock["price"])
        formatted_stock["value"] = usd(stock["value"])
        formatted_stocks.append(formatted_stock)

    formatted_cash = usd(cash)
    formatted_total_value = usd(total_value)

    return render_template("index.html", stocks=formatted_stocks, cash=formatted_cash, total_value=formatted_total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol", 400)

        try:
            shares = int(shares)
            if shares <= 0:
                return apology("must provide positive number of shares", 400)
        except ValueError:
            return apology("shares must be a positive integer", 400)

        stock = lookup(symbol)
        if stock is None:
            return apology("invalid symbol", 400)

        price = stock["price"]
        cost = price * shares

        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]
        if cash < cost:
            return apology("can't afford", 400)

        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", cost, session["user_id"])

        db.execute("INSERT INTO transactions (user_id, type, symbol, shares, price) VALUES (?, 'buy', ?, ?, ?)",
                   session["user_id"], symbol, shares, price)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("""
        SELECT type, symbol, shares, price, timestamp
        FROM transactions
        WHERE user_id = ?
        ORDER BY timestamp DESC
    """, session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        if not quote:
            return apology("invalid symbol", 400)
        return render_template("quote.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        if len(rows) != 0:
            return apology("username already exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get(
            "username"), generate_password_hash(request.form.get("password")))

        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        session["user_id"] = rows[0]["id"]

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("missing symbol", 400)
        shares = request.form.get("shares")
        if not shares:
            return apology("missing shares", 400)

        look_up = lookup(symbol)

        rows = db.execute(
            "SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        cnt = rows[0]['SUM(shares)']
        if int(shares) > cnt:
            return apology("too many shares", 400)

        db.execute("INSERT INTO transactions (user_id, type, symbol, shares, price) VALUES (?, 'sell', ?, ?, ?)",
                   session["user_id"], symbol, int(shares) * (-1), look_up["price"])

        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        res = rows[0]["cash"] + look_up["price"] * int(shares)
        db.execute("UPDATE users SET cash = ? WHERE id = ?", res, session["user_id"])

        flash('Sold!')

        return redirect("/")

    symbols = db.execute(
        "SELECT DISTINCT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])

    return render_template("sell.html", symbols=symbols)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Allow user to change password"""
    if request.method == "POST":
        if not request.form.get("current_password"):
            return apology("must provide current password", 403)

        if not request.form.get("new_password"):
            return apology("must provide new password", 403)

        if not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        if not check_password_hash(user[0]["hash"], request.form.get("current_password")):
            return apology("invalid current password", 403)

        if request.form.get("new_password") != request.form.get("confirmation"):
            return apology("new passwords do not match", 403)

        hash = generate_password_hash(request.form.get("new_password"))

        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, session["user_id"])

        return redirect("/")

    else:
        return render_template("change_password.html")
