import requests
import json

import dateutil.parser
from datetime import datetime

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure


date = f" ({datetime.now().strftime('%Y-%m-%d')})"

figure(figsize=(6.4,3.6),dpi=120)

DISPLAY = False


def download_user_information():
    content = []
    for page in range(1, 10000):
        url = f"https://dmoj.ca/api/v2/users?page={page}"
        print(url)
        r = requests.get(url)
        if r.status_code != 200:
            print("Error:", r.status_code)
            break
        else:
            s = str(r.content, "utf-8")
            data = json.loads(s)
            content.append(data)
    print("Writing JSON file...")
    s = json.dumps(content)
    open("users.json", "w").write(s)
    print("Writing JSON file completed.")


def load_data():
    s = open("users.json", "r").read()
    data = json.loads(s)
    global date
    date = f" ({dateutil.parser.isoparse(data[0]['fetched']).strftime('%Y-%m-%d')})"
    res = []
    for page in data:
        res += page['data']['objects']
    return res


def plot_rating_pp_graph(data):
    users = []
    for user in data:
        if user['rating']!=None:
            users.append((user['performance_points'],user['rating']))
    plt.clf()
    plt.title("DMOJ Users Rating-PP Graph" + date)
    plt.axis([0, 1000, 0, 4000])
    plt.xlabel("Performance Points")
    plt.ylabel("Rating")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro", markersize=3)
    plt.savefig("users/plot_rating_pp_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_pp_points_graph(data):
    users = []
    for user in data:
        if user['points']>1e-4:
            users.append((user['points'],user['performance_points']))
    plt.clf()
    plt.title("DMOJ Users PP-Points Graph" + date)
    plt.xlabel("Points")
    plt.ylabel("Performance Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro", markersize=3)
    plt.savefig("users/plot_pp_points_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_points_solved_graph(data):
    users = []
    for user in data:
        if user['problem_count']>0:
            users.append((user['problem_count'],user['points']))
    plt.clf()
    plt.title("DMOJ Users Points-Solved Graph" + date)
    plt.xlabel("Problem Count")
    plt.ylabel("Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro", markersize=3)
    plt.savefig("users/plot_points_solved_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_pp_solved_graph(data):
    users = []
    for user in data:
        if user['problem_count']>0:
            users.append((user['problem_count'],user['performance_points']))
    plt.clf()
    plt.title("DMOJ Users PP-Solved Graph" + date)
    plt.xlabel("Problem Count")
    plt.ylabel("Performance Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro", markersize=3)
    plt.savefig("users/plot_pp_solved_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_pp_rank_graph(data):
    users = []
    for user in data:
        if user['performance_points'] > 0:
            users.append(user['performance_points'])
    plt.clf()
    lin = plt.subplot(121)
    lin.plot(sorted(users, reverse=True), "r")
    lin.set_yscale('linear')
    log = plt.subplot(122)
    log.plot(sorted(users, reverse=True), "r")
    log.set_yscale('log')
    plt.gcf().suptitle("DMOJ Users PP-Rank Graph" + date)
    lin.set_xlabel("Rank by PP")
    lin.set_ylabel("Performance Points")
    log.set_xlabel("Rank by PP")
    plt.savefig("users/plot_pp_rank_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_rating_rank_graph(data):
    users = []
    for user in data:
        if user['rating'] != None:
            users.append(user['rating'])
    plt.clf()
    plt.title("DMOJ Users Rating-Rank Graph" + date)
    plt.xlabel("Rank by Rating")
    plt.ylabel("Rating")
    plt.plot(sorted(users, reverse=True), "r")
    plt.savefig("users/plot_rating_rank_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_solved_rank_graph(data):
    users = []
    for user in data:
        if user['problem_count'] > 0:
            users.append(user['problem_count'])
    plt.clf()
    lin = plt.subplot(121)
    lin.plot(sorted(users, reverse=True), "r")
    lin.set_yscale('linear')
    log = plt.subplot(122)
    log.plot(sorted(users, reverse=True), "r")
    log.set_yscale('log')
    plt.gcf().suptitle("DMOJ Users Solved-Rank Graph" + date)
    lin.set_xlabel("Rank by Problem Count")
    lin.set_ylabel("Problem Count")
    log.set_xlabel("Rank by Problem Count")
    plt.savefig("users/plot_solved_rank_graph.png", dpi=150)
    if DISPLAY: plt.show()


def plot_rating_histogram(data):
    users = []
    for user in data:
        if user['rating'] != None:
            users.append(user['rating'])
    plt.clf()
    plt.title("DMOJ Rating Histogram" + date)
    bins = list(range(50*(min(users)//50-1),50*(max(users)//50+1),50))
    plt.hist(list(filter(lambda x: x<1000, users)), bins, color='#999')
    plt.hist(list(filter(lambda x: 1000<=x<1300, users)), bins, color='#00a900')
    plt.hist(list(filter(lambda x: 1300<=x<1600, users)), bins, color='blue')
    plt.hist(list(filter(lambda x: 1600<=x<1900, users)), bins, color='purple')
    plt.hist(list(filter(lambda x: 1900<=x<2400, users)), bins, color='#ffb100')
    plt.hist(list(filter(lambda x: 2400<=x<3000, users)), bins, color='red')
    plt.hist(list(filter(lambda x: x>3000, users)), bins, color='darkred')
    plt.savefig("users/plot_rating_histogram.png", dpi=150)
    if DISPLAY: plt.show()


#download_user_information()

data = load_data()

if not DISPLAY:
    plot_rating_pp_graph(data)
    plot_pp_points_graph(data)
    plot_points_solved_graph(data)
    plot_pp_solved_graph(data)
    plot_pp_rank_graph(data)
    plot_rating_rank_graph(data)
    plot_solved_rank_graph(data)
    plot_rating_histogram(data)
