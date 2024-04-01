retries = 0

while True:
    import time
    time_of_fail = time.time()

    try:

        import time

        from parinya import LINE
        from selenium import webdriver
        from selenium.webdriver.common.by import By
        from selenium.webdriver.chrome.options import Options
        from selenium.webdriver.common.keys import Keys
        from selenium.webdriver.chrome.service import Service

        service = Service(executable_path=r'/usr/bin/chromedriver')
        options = webdriver.ChromeOptions()
        options.add_argument('--headless')
        options.add_argument('--no-sandbox')
        options.add_argument('--disable-dev-shm-usage')

        line_token = "*******************************************"
        user_name = "*********************"
        password = "********"

        driver = webdriver.Chrome(service=service, options=options)
        driver.get("https://owner.justwash.asia/#/washing-machine")

        time.sleep(8)
        login1 = driver.find_element(By.XPATH, '//*[@id="app"]/div[2]/div[2]/div/div/div[1]/div/form/div[1]/input')
        login1.send_keys(user_name)
        login2 = driver.find_element(By.XPATH, '//*[@id="app"]/div[2]/div[2]/div/div/div[1]/div/form/div[2]/input')
        login2.send_keys(password)
        time.sleep(8)
        enter = driver.find_element(By.XPATH, '//*[@id="app"]/div[2]/div[2]/div/div/div[1]/div/form/div[3]/button')
        enter.send_keys(Keys.ENTER)
        # enter.click()
        time.sleep(8)
        main = driver.find_element(By.XPATH, '//*[@id="sidenav-collapse-main"]/ul[1]/li[2]/a')
        main.send_keys(Keys.RETURN)

        time.sleep(8)

        while True:
            line = LINE(line_token)
            index = 0
            name_of_the_machine = []

            name_machine = driver.find_element(By.TAG_NAME, "tbody")
            name_machine = name_machine.find_elements(By.TAG_NAME, "tr")

            for i in name_machine:
                a = i.find_element(By.TAG_NAME, "td")
                a = a.text
                name_of_the_machine.append(a)

            status_of_the_machine = []

            status = driver.find_elements(By.CLASS_NAME, "status")
            for i in status:
                a = i.text
                status_of_the_machine.append(a)

            for i in status_of_the_machine:
                if i == "Offline":
                # if i == "Ready":
                    line_output = name_of_the_machine[index].split("\n")
                    line.sendtext(f"\nKhon Kean\n{line_output[0]} Error")
                    index += 1

            time.sleep(60)

    except:

        from parinya import LINE
        line_token = "*******************************************"
        line = LINE(line_token)

        if retries > 5:
            line.sendtext("ERROR (Khon Kean), Ming check your Script!!!")

        if round(time.time() - time_of_fail) < 300:
            retries += 1

        time.sleep(100)
        continue