import requests
import json

def comet_send_message(channel, id, username, avatar, text, isVote, qAuthor, countAnswers):
    url = 'http://127.0.0.1:80/comet-publish/?id=' + channel
    body = json.dumps({'id':id,'username':username,'avatar':avatar,'text':text,'isVote':isVote,'qAuthor':qAuthor,'countAnswers': countAnswers})
    # print(body)
    request = requests.post(url, body)
    # print(request.text)
    return request