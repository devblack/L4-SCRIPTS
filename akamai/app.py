import requests


class MainApp():

    def __init__(self):
        key = input('> Enter your master-key: ')
        #api here
        url_check = "http://your_url/py_auth.php"
        data = dict(athena_key=key)
        try:
            req = requests.post(url_check, data=data)
            if req.text == "error0":
                print("> Enter you master key.")
            elif req.text == "error1":
                print("> Master key invalid.")
            elif req.text == "error2":
                print("> Master key is disable, contact to the administrator.")
            elif req.text == "true":
                print("> Success, you are welcome!")
            else:
                print("> Unknown error, contact to the administrator.")
        except requests.exceptions.ConnectionError:
            print("> Connection problem.")

if __name__ == '__main__':
    MainApp()
