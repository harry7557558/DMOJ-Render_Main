import requests
import json
import matplotlib.pyplot as plt

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
    res = []
    for page in data:
        res += page['data']['objects']
    return res

def plot_pp_rating_graph(data):
    users = []
    for user in data:
        if user['rating']!=None:
            users.append((user['performance_points'],user['rating']))
    plt.title("DMOJ User PP-Rating Graph")
    plt.axis([0, 1000, 0, 4000])
    plt.xlabel("Performance Points")
    plt.ylabel("Rating")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro")
    plt.show()

def plot_pp_points_graph(data):
    users = []
    for user in data:
        if user['points']>1e-4:
            users.append((user['points'],user['performance_points']))
    plt.title("DMOJ User PP-Points Graph")
    plt.xlabel("Points")
    plt.ylabel("Performance Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro")
    plt.show()

def plot_solved_points_graph(data):
    users = []
    for user in data:
        if user['problem_count']>0:
            users.append((user['problem_count'],user['points']))
    plt.title("DMOJ User Solved-Points Graph")
    plt.xlabel("Problem Count")
    plt.ylabel("Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro")
    plt.show()

def plot_solved_pp_graph(data):
    users = []
    for user in data:
        if user['problem_count']>0:
            users.append((user['problem_count'],user['performance_points']))
    plt.title("DMOJ User Solved-PP Graph")
    plt.xlabel("Problem Count")
    plt.ylabel("Performance Points")
    plt.plot([u[0] for u in users], [u[1] for u in users], "ro")
    plt.show()


#download_user_information()

plot_pp_rating_graph(load_data())
