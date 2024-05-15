import random
from flask import Flask, render_template, request, redirect, url_for

app = Flask(__name__)

BOARD_SIZE = 5
NUM_SHIPS = 3

class GameBoard:
    def __init__(self):
        self.board = [['-' for _ in range(BOARD_SIZE)] for _ in range(BOARD_SIZE)]

    def print_board(self):
        return "\n".join(" ".join(row) for row in self.board)

def is_valid_coordinate(x, y):
    return 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE

def place_ships(board, num_ships):
    for _ in range(num_ships):
        while True:
            x, y = random.randint(0, BOARD_SIZE - 1), random.randint(0, BOARD_SIZE - 1)
            if board[x][y] != 'S':
                break
        board[x][y] = 'S'

def player_turn(computer_board, x, y):
    if computer_board.board[x][y] == 'S':
        computer_board.board[x][y] = 'X'
        return "Hit!"
    elif computer_board.board[x][y] == '-':
        computer_board.board[x][y] = 'O'
        return "Miss!"
    else:
        return "You've already fired at this location. Try again."

def computer_turn(player_board):
    while True:
        x, y = random.randint(0, BOARD_SIZE - 1), random.randint(0, BOARD_SIZE - 1)
        if player_board.board[x][y] == '-':
            break

    if player_board.board[x][y] == 'S':
        player_board.board[x][y] = 'X'
        return f"Computer hits at ({x}, {y})!"
    elif player_board.board[x][y] == '-':
        player_board.board[x][y] = 'O'
        return f"Computer misses at ({x}, {y})."
    else:
        return f"Computer already fired at ({x}, {y})."

def is_game_over(board):
    for row in board.board:
        if 'S' in row:
            return False
    return True

player_board = GameBoard()
computer_board = GameBoard()
place_ships(player_board.board, NUM_SHIPS)
place_ships(computer_board.board, NUM_SHIPS)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/attack', methods=['POST'])
def attack():
    if 'x' in request.form and 'y' in request.form:
        x = int(request.form['x'])
        y = int(request.form['y'])
        if is_valid_coordinate(x, y):
            message = player_turn(computer_board, x, y)
            if is_game_over(computer_board):
                return render_template('result.html', message=message, winner="Player")
            message += "\n" + computer_turn(player_board)
            if is_game_over(player_board):
                return render_template('result.html', message=message, winner="Computer")
            return render_template('game.html', player_board=player_board.print_board(), message=message)
    return redirect(url_for('index'))

if __name__ == "__main__":
    app.run(debug=True)
