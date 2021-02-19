import requests_html

def getCommentVote(url):
    session = requests_html.HTMLSession()
    r = session.get(url)
    cmt = r.html.find(url[url.find('#'):],first=True)
    if cmt==None: return 'deleted'
    vote = cmt.find('.comment-score',first=True)
    return ("%+d"%int(vote.text)).replace('+0','0')

comments = [
    [
    "https://dmoj.ca/problem/dmpg18g5#comment-11746",
    "https://dmoj.ca/problem/bf3hard#comment-11948",
    "https://dmoj.ca/problem/det#comment-12176",
    "https://dmoj.ca/problem/helloworldhard#comment-11390",
    "https://dmoj.ca/problem/helloworldhard#comment-11706",
    "https://dmoj.ca/problem/mockecoo14p1#comment-11971",
    "https://dmoj.ca/post/103-point-system-rework#comment-11819",
    "https://dmoj.ca/problem/angieandfunctions/editorial#comment-11831",
    "https://dmoj.ca/contest/pib20#comment-11784",
    "https://dmoj.ca/problem/dmopc19c6p1#comment-11955",
    "https://dmoj.ca/problem/ccoqr16p3#comment-12067",
    "https://dmoj.ca/problem/2drmq#comment-11850",
    "https://dmoj.ca/problem/ccc18s1#comment-11680",
    "https://dmoj.ca/problem/ccc20s2#comment-11890",
    "https://dmoj.ca/problem/raytracing#comment-11707",
    "https://dmoj.ca/problem/wac2p4#comment-11387",
    "https://dmoj.ca/problem/dmpg16s5#comment-10695",
    "https://dmoj.ca/problem/dmpg15b6#comment-11744",
    "https://dmoj.ca/problem/ccc06s2#comment-10609",
    "https://dmoj.ca/problem/ccc00s5#comment-11896",
    "https://dmoj.ca/problem/ccc01j2#comment-10556",
    "https://dmoj.ca/problem/wac3p3#comment-11446",
    "https://dmoj.ca/problem/ccc18s3#comment-12049",
    "https://dmoj.ca/problem/ccc20s5#comment-11874"
    ],
    [
    "https://dmoj.ca/problem/miraclesort#comment-12876",
    "https://dmoj.ca/problem/piggy#comment-12479",
    "https://dmoj.ca/contest/cco20d1#comment-12469",
    "https://dmoj.ca/problem/tle18p1#comment-12374",
    "https://dmoj.ca/problem/tle18p1#comment-12639",
    "https://dmoj.ca/problem/ccc00s5#comment-12445",
    "https://dmoj.ca/problem/ccc19s3/editorial#comment-12513",
    "https://dmoj.ca/problem/helloworldhard#comment-12408"
    ],
    [
    "https://dmoj.ca/problem/ccc09s1#comment-11805",
    "https://dmoj.ca/problem/ccc11s3#comment-11818",
    "https://dmoj.ca/problem/det#comment-12184",
    "https://dmoj.ca/problem/ccc19s3/editorial#comment-12560",
    "https://dmoj.ca/contest/dmopc20c2#comment-13859",
    "https://dmoj.ca/problem/wac6p4/editorial#comment-14260"
    ],
    [
    "https://dmoj.ca/problem/piggy#comment-12480",
    "https://dmoj.ca/problem/ccc19s3/editorial#comment-12550",
    "https://dmoj.ca/problem/ccc00s5#comment-13284",
    "https://dmoj.ca/contest/ectf20#comment-12930",
    "https://dmoj.ca/problem/noi05p6#comment-13374",
    "https://dmoj.ca/contest/rte3#comment-13546",
    "https://dmoj.ca/contest/dmopc20c2#comment-13763"
    ]
    ]

for user in comments:
    for cmt in user:
        print(cmt + ' (' + getCommentVote(cmt) + ')\n')
    print('--------\n')


