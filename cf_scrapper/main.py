from selenium import webdriver
from selenium.webdriver.common.by import By
import time
import requests

driver = webdriver.Chrome("D:/Softwares/WebDrivers/chromedriver.exe")

handle = "partha_paul"
solutions_path = "D:/test"
response = requests.get("https://codeforces.com/api/user.status?handle="+handle+"&from=1")
stat = response.json()['result']
done = []

counter = 0
for submission in stat:
    problem_name = submission['problem']['name']
    verdict = submission['verdict']
    if verdict == "OK" and ( problem_name not in done ):
        done.append(problem_name)
        solution_url = "https://codeforces.com/contest/"+str(submission['problem']['contestId'])+"/submission/"+str(submission['id'])
        driver.get(solution_url)
        try:
            element = driver.find_element(By.ID,"program-source-text")
            solution = element.text
            with open(solutions_path + "/" + handle + ".txt", "a") as file:
                file.write(solution)
            print("Problem {}: {} Done".format(counter, problem_name))
        except Exception as e:
            print("Problem {}: {} Done".format(counter, "Some error occured"))
        counter += 1