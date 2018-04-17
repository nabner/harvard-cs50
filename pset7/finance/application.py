from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

import datetime
import logging

from helpers import *

# configure application
app=Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"]="no-cache, no-store, must-revalidate"
        response.headers["Expires"]=0
        response.headers["Pragma"]="no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"]=usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"]=mkdtemp()
app.config["SESSION_PERMANENT"]=False
app.config["SESSION_TYPE"]="filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db=SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # Sometimes the alphavantage API has issues, if it is unable to provide the neccessary
    # data to present the portfolio, route the user to the "quote" template
    # https://www.alphavantage.co/documentation/
    try:
        user=db.execute("SELECT username FROM users WHERE id=:id", id=session['user_id'])
        logging.info(user)

        # Retrieve the stock symbol(s) and the corresponding number of shares from the portfoilo table
        portfolio_stocks=db.execute("SELECT stock_symbol, stock_quantity FROM portfolio \
            WHERE users_user_id=:id", id=session['user_id'])

        logging.info("portfolio_stocks")
        logging.info(portfolio_stocks)

        logging.info("The user's ID is %d" %session['user_id'])

        portfolio=update_portfolio(session['user_id'])

        # This variable will calculate the total value of the portfolio (investments + cash)
        portfolio_value=0

        # Get the current value of cash from the "users" table
        current_cash=db.execute("SELECT cash FROM users WHERE id=:id;", id=session["user_id"])

        logging.debug(current_cash)

        current_cash=float(current_cash[0]['cash'])

        # If there is anything in the user's stock portfolio, add the value to
        # the "portfolio_value" variable
        if portfolio:

            for stock in portfolio:

                portfolio_value += float(stock['total'])

        grand_total=current_cash + portfolio_value

        logging.debug("The grand total is: %d" %grand_total)

        return render_template('index.html', current_cash=current_cash, grand_total=grand_total, \
            portfolio_value=portfolio_value, portfolio=portfolio)

    # If there was an issue with the alphavantage API, reroute user to the "quote" template
    except:
        return render_template("quote.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # If user reached route via GET (as by clicking on a link)
    # Show the buy.html template for them to choose stock and quantity
    if request.method == "GET":
        return render_template("buy.html")

    # If user reached route via POST (as by submitting a form)
    # Try to complete the transaction
    else:

        # Variable for the stock symbol the user entered in the form
        stock=lookup(request.form.get("symbol"))

        # Variable for the number of shares the user wants to buy
        num_shares=request.form.get("shares")

        # Ensure valid stock symbol was submitted
        if not stock:
            return display_error("buy.html", error_message="You must provide a \
                correct stock symbol.")

        # Ensure the user did not leave the "shares" input field blank
        if not num_shares:
            return display_error("buy.html", error_message="You must enter \
            the number of shares you wish to buy. It must be a whole number \
            greater than zero.")

        try:
            # Ensure num_shares is being stored as an integer
            num_shares=int(num_shares)
        except:
            # Ensure the user entered an integer in the "shares" input field
            if type(num_shares) != int:
                return display_error("buy.html", error_message="The number \
                of shares entered must be a whole integer.")

        # Ensure valid quantity of shares was submitted
        if num_shares <= 0:
            return display_error("buy.html", error_message="You must enter \
            the number of shares you wish to buy. It must be a whole number \
            greater than zero.")

        # Can the user afford the stock?
        # Retrieve the user's current amount of cash on hand from "users" database
        current_cash=db.execute("SELECT cash FROM users WHERE id=:id;", id=session["user_id"])

        # Current cash variable needs to be a float because
        # Stock prices will have decimals
        current_cash=float(current_cash[0]['cash'])


        # If the user does not have enough cash, return an error that funds are not sufficient
        if (num_shares * stock['price']) > current_cash:
            return display_error("buy.html", error_message="Not enough \
                funds to complete this transaction.")

        # If the user has enough cash, proceed with the purchase
        if (num_shares * stock['price']) <= current_cash:

            now=datetime.datetime.now()

            # Update the "history" table to show the purchase
            db.execute("INSERT INTO history (symbol, shares, price, user_ID, transaction_type, history_datetime) \
                VALUES (:symbol, :shares, :price, :id, 'buy', :history_datetime)", \
                symbol=stock['symbol'], shares=num_shares,
                price=usd(stock['price']), id=session['user_id'], history_datetime=str(now))

            # Update user's cash value in the "users" table
            db.execute("UPDATE users SET cash=cash - :purchase_price WHERE id=:id", \
                purchase_price=num_shares * stock['price'], id=session['user_id'])

            # "users_stock" will be used to check if the user already owns stock in that company
            users_stock=db.execute("SELECT stock_quantity FROM portfolio WHERE stock_symbol=:symbol AND users_user_id=:id", \
                symbol=stock['symbol'], id=session['user_id'])

            # If the user does not own stock in that company, add a row to the portfolio table
            if not users_stock:
                db.execute("INSERT INTO portfolio (users_user_id, stock_symbol, stock_name, stock_quantity, total) \
                    VALUES (:id, :symbol, :name, :quantity, :total)", \
                    id=session['user_id'], symbol=stock['symbol'], name=stock['name'], quantity=num_shares, \
                    total=num_shares * stock['price'])

            # If the user DOES own stock in that company, EDIT the existing row in the portfolio table
            else:
                db.execute("UPDATE portfolio SET stock_quantity=stock_quantity + :quantity, total=total + :total \
                WHERE users_user_id=:id AND stock_symbol=:symbol",
                quantity=num_shares, total=num_shares * stock['price'], \
                id=session['user_id'], symbol=stock['symbol'])

            # Determine the correct word to use on the congratulations screen
            if num_shares > 1:
                share_word="shares"
            else:
                share_word="share"

            # Try to redirect the user to index.html so they can review their full portfoilo
            # including the new purchase, but there are often issues with the API
            try:
                return index()

            # If index.html cannot be reached, redirect the user to a success page
            # so the user knows the transaction was successful
            except:
                return render_template("success.html", transaction_type="bought", symbol=stock['symbol'], \
                share_word=share_word, number_of_shares=num_shares, transaction_amount=num_shares * stock['price'])


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # Retrieve the history table
    history_all=db.execute("SELECT * FROM history WHERE user_ID=:id", id=session["user_id"])

    # Jinja will loop through the history table and present it accordingly
    return render_template("history.html", history_all=history_all)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # Clear any user_id that is currently logged in (if any)
    session.clear()

    # If user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return display_error("login.html", error_message="Must provide a username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return display_error("login.html", error_message="Must provide a password")

        # Query the "users" table for username
        rows=db.execute("SELECT * FROM users WHERE username=:username", username=request.form.get("username"))

        # Ensure the username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return display_error("login.html", error_message="Invalid username and/or password")

        # Remember which user has logged in
        session["user_id"]=rows[0]["id"]

        # Redirect user to home page
        return redirect(url_for("index"))

    # Else, if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # Clear the user_id that is currently logged in
    session.clear()

    # Redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If user reached route via POST (as by submitting a form via POST)
    # Lookup the current value of that stock
    if request.method == "POST":
        quote=lookup(request.form.get("symbol"))

        # Ensure a correct stock symbol was provided
        if not quote:
            return display_error("quote.html", error_message="Stock symbol was not valid.")
        else:
            quote['price']=usd(quote['price'])
            return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # If user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return display_error("register.html", error_message="Must provide username.")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return display_error("register.html", error_message="Must provide password.")

        # Ensure password was submitted twice
        elif not request.form.get("confirmation"):
            return display_error("register.html", error_message="Must provide password again.")

        # Ensure the two passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return display_error("register.html", error_message="Passwords do not match.")

        # Check to see if the username already exists in the users table
        all_users=db.execute("SELECT username FROM users")

        for user in all_users:
            if user == request.form.get("username"):
                return display_error("register.html", error_message="Username already exists.")

        # Insert the new user into the "users" table
        # Store the users password as a hash
        result=db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", \
                            username=request.form.get("username"), \
                            hash=pwd_context.hash(request.form.get("password")))

        # Ensure the storing of the new username and password was successful
        if not result:
            return display_error("register.html", error_message="Username already exists.")

        # Remember which user has logged in
        session["user_id"]=result

        # Redirect user to home page
        return redirect(url_for("index"))

    # Else, if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # This variable will be passed to the html template for the user
    # to choose the stock to sell based on what stocks they own
    symbol_option=db.execute("SELECT stock_symbol FROM portfolio WHERE users_user_id=:id", \
        id=session["user_id"])

    # If user reached route via GET (as by clicking on a link)
    # Show the buy.html template for user to choose stock and quantity
    if request.method == "GET":

        logging.debug("GET method application.py symbol_option")
        logging.debug(symbol_option)

        return render_template("sell.html", symbol_option=symbol_option)

    # If user reached route via POST (as by submitting a form)
    # Try to complete the transaction
    else:

        # Variable for the number of shares the user wants to sell
        num_shares=request.form.get("shares")

        # Variable for the stock symbol of shares the user submitted
        user_symbol=request.form.get("symbol")

        logging.info("user_symbol for sell: %s" %user_symbol)

        # Ensure the user did not leave the "shares" input field blank
        if not num_shares:
            return display_error("sell.html", \
                error_message="You must enter the number of shares you wish to \
                sell. It must be a whole number greater than zero.")

        # If the user provided the number of shares to sell, ensure it is an
        # int or convertable to an int
        try:
            # Ensure num_shares is being stored as an integer
            num_shares=int(num_shares)
        except:
            # Ensure the user entered an integer in the "shares" input field
            if type(num_shares) != int:
                return display_error("sell.html", \
                    error_message="The number of shares entered must be a \
                    whole integer.")

        # Ensure a valid quantity of shares to sell was submitted
        if num_shares <= 0:
            return display_error("sell.html", \
                error_message="You must enter the number of shares you wish to \
                sell. It must be a whole number greater than zero.")

        # Ensure the user selected a valid number of shares based on their stock ownership
        current_shares=db.execute("SELECT stock_quantity FROM portfolio WHERE \
            stock_symbol=:symbol AND users_user_id=:id", symbol=user_symbol, \
            id=session["user_id"])

        logging.debug(current_shares)

        current_shares=(current_shares[0]['stock_quantity'])

        logging.debug(current_shares)

        if num_shares > current_shares:
            return display_error("sell.html", \
                error_message="You cannot sell {:d} share(s) because your portfolio \
                currently holds {:d} share(s) of that stock.".format(num_shares, \
                current_shares))

        # Get the current market value of the stock
        stock=lookup(user_symbol)

        logging.debug(stock)

        if stock == None:
            return display_error("sell.html", \
                error_message="There is an error with the lookup function. \
                Please try again later")

        # At this point, we know the user has submitted a valid symbol and
        # valid number of shares based on what they own, proceed with the sale

        # Update the quantity of that stock in the portfolio table
        db.execute("UPDATE portfolio SET stock_quantity=stock_quantity - :quantity_sold \
            WHERE stock_symbol=:symbol AND users_user_id=:id", quantity_sold=num_shares, \
            symbol=user_symbol, id=session["user_id"])

        # Get the current market value of the stock
        #stock=lookup(user_symbol)

        #logging.debug(stock)

        # Use the current market value to update the cash in the users table
        db.execute("UPDATE users SET cash=cash + :sale_price WHERE id=:id", \
            sale_price=(num_shares * stock['price']), id=session['user_id'])

        now=datetime.datetime.now()

        # Document the sale in the history table
        db.execute("INSERT INTO history (symbol, shares, price, user_ID, transaction_type, history_datetime) \
            VALUES (:symbol, :shares, :price, :id, 'sale', :history_datetime)", \
            symbol=user_symbol, shares=num_shares, price=usd(stock['price']), \
            id=session['user_id'], history_datetime=str(now))

        # Determine the correct word to use on the congratulations screen
        if num_shares > 1:
            share_word="shares"
        else:
            share_word="share"


    return render_template("success.html", transaction_type="sold", \
        transaction_amount=num_shares*stock['price'], \
        symbol=user_symbol, share_word=share_word, number_of_shares=num_shares)

@app.route("/funds", methods=["GET", "POST"])
@login_required
def funds():
    """Add cash to the users account."""

    # If user reached route via GET (as by clicking on a link)
    # Show the funds.html template for user to enter the amount of cash
    if request.method == "GET":

        return render_template("funds.html")

    # If user reached route via POST (as by submitting a form)
    # Try to complete the transaction
    else:

        funds=request.form.get("funds")

        # Ensure the user did not leave the "funds" input field blank
        if not funds:
            return display_error("funds.html", error_message="You must enter \
            amount of cash you wish to add to your account.")

        # If the user provided the amount of cash, ensure it is an
        # int or convertable to an int
        try:
            # Ensure funds is being stored as an integer
            funds=int(funds)
        except:
            # Ensure the user entered an integer in the "funds" input field
            if type(funds) != int:
                return display_error("funds.html",
                    error_message="The amount of cash entered must be a \
                    whole integer.")

        # Ensure the value of "funds" is greater than 0
        if funds <= 0:
            return display_error("funds.html",  \
                error_message="You must enter the amount of cash you wish to \
                add to your account. It must be a whole number greater than zero.")

        # Update cash in the "users" table
        db.execute("UPDATE users SET cash=cash + :new_funds WHERE id=:id", \
            new_funds=funds, id=session['user_id'])

        # Pull the updated value of "cash" to present to the user
        updated_cash=db.execute("SELECT cash FROM users WHERE id=:id", \
            id=session['user_id'])

        # Convert the cash value to an int so it can be formatted as $usd
        updated_cash=float(updated_cash[0]['cash'])

        logging.debug("updated_cash")
        logging.debug(updated_cash)

        now=datetime.datetime.now()

        # Update the "history" table to show the transaction
        db.execute("INSERT INTO history (symbol, shares, price, user_ID, transaction_type, history_datetime) \
            VALUES (:symbol, :shares, :price, :id, 'cash_transaction', :history_datetime)", \
            symbol='N/A', shares=0, price=usd(funds), id=session['user_id'], \
            history_datetime=str(now))

        return render_template("funded.html", transaction_amount=funds, cash=updated_cash)

