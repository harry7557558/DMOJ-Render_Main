import requests
import json

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure

import sys


def load_data(contest_id):
    url = f"https://dmoj.ca/api/v2/contest/{contest_id}"
    r = requests.get(url)
    if r.status_code != 200:
        sys.stderr.write("Error: " + str(r.status_code) + '\n')
        return
    else:
        s = str(r.content, "utf-8")
        data = json.loads(s)
    data = json.loads(s)
    people = data['data']['object']['rankings']
    ratings = []
    for person in people:
        old_rating = person['old_rating']
        new_rating = person['new_rating']
        if new_rating != None:
            if old_rating == None:
                old_rating = 1200
            ratings.append([old_rating, new_rating])
    return np.array(ratings)


def plot_ratings(contest_id, data, deg=0, display=True):
    print(contest_id)
    plt.clf()
    plt.title(contest_id)
    plt.xlabel("Old rating")
    plt.ylabel("Rating change")
    old_ratings = data[:, 0]
    rating_changes = data[:, 1] - old_ratings
    print("Average rating change:", np.average(rating_changes))
    print("Rating change sigma:", np.sqrt(np.cov(rating_changes)))
    x0 = min(old_ratings)
    x1 = max(old_ratings)
    plt.plot([x0, x1], [0, 0], 'r', dashes=[5, 5])
    if deg >= 1:
        poly = np.polyfit(old_ratings, rating_changes, deg)
        if deg == 1:
            print("Regression slope:", poly[-2])
        poly_x = np.arange(x0, x1, 2)
        poly_y = np.polyval(poly, poly_x)
        plt.plot(poly_x, poly_y, 'b', dashes=[5, 5])
    plt.plot(old_ratings, rating_changes, 'co')
    if display:
        plt.show()
    print(end='\n')


def plot_contest(contest_id):
    data = load_data(contest_id)
    plot_ratings(contest_id, data, deg=1)


def plot_contests(contest_ids):
    sys.stdout = open("rating_change/stdout.txt", "w")
    for contest_id in contest_ids:
        sys.stderr.write(contest_id + '\n')
        data = load_data(contest_id)
        plot_ratings(contest_id, data, deg=1, display=False)
        plt.savefig(f"rating_change/{contest_id}.png", dpi=150)
        sys.stdout.flush()
    sys.stdout.close()


if __name__ == "__main__":

    #plot_contest('acc8')

    plot_contests(['acc7',
                   'dmopc20c5',
                   'aac2',
                   'dmopc20c6',
                   'dmopc20c7',
                   'canada21',
                   'cpc21c1',
                   'aac3',
                   'acc8'])
