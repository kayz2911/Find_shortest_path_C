
from io import TextIOWrapper
import json, os, random
from sys import stdout
import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
driver = webdriver.Chrome()

# url = "flash_sale"
cur_dir = os.getcwd()

def scrapingWeb(url : str) :
    driver.get(url)
    # move down to the end of page for full loading
    for i in range(5) :
        webdriver.ActionChains(driver).key_down(Keys.SPACE).perform()
        time.sleep(1)
    # for special case, url is flash_sale
    if url == cur_dir + "/flash_sale" :
        items = driver.find_elements_by_class_name("flash-sale-item-card__item-name")
        elements_parent = driver.find_elements_by_class_name("flash-sale-item-card__lower-left")
        prices = [ i.find_element_by_class_name("item-price-number") for i in elements_parent]
    else :
        items = driver.find_elements_by_class_name("O6wiAW")
        prices = driver.find_elements_by_class_name("_2lBkmX")
    final_name, final_price = [], []
    for i, j in zip(items, prices) :
        # remove useless part of product's name
        name = str(i.text).splitlines()
        name_most = name[0].split(",")
        exact_price = str(j.text).splitlines()
        # remove '-' and 'd' from exact_price
        exact_price = exact_price[-1].split("-")
        price_most = exact_price[-1].split("₫")
        final_name.append(name_most[0])
        final_price.append(price_most[-1])

    return [final_name, final_price]
    

def extract_address_from_json(file : str) :
    with open(file, "r") as reader :
        r = reader.read()
        json_obj = json.loads(r)
    
    code_address = dict()
    for i in json_obj :
        key = i.get('code')
        val = i.get('full_name')
        code_address[key] = val
    return code_address


def append_data_to_file(url : str, json_file : str, output : TextIOWrapper) :
    code_address = extract_address_from_json(json_file)
    # print name, price out to a file
    out = scrapingWeb(url)
    items, prices = out[0], out[1]
    for i, j in zip(items, prices) :
        # random code of address
        random_code = random.choice(list(code_address.keys()))
        print(f"{code_address.get(random_code)}, {i}, {j}", file=output)
       
    return 1    

if __name__ == "__main__" :
    
    sections = [
     "Thiết-Bị-Điện-Tử-cat.2365", "Sức-Khỏe-Sắc-Đẹp-cat.160",
     "Nhà-Sách-Online-cat.10256","Ô-tô-xe-máy-xe-đạp-cat.12494",
     "Chăm-sóc-thú-cưng-cat.18977", "Mẹ-Bé-cat.163",
     "Phụ-Kiện-Thời-Trang-cat.80","Máy-tính-Laptop-cat.13030"] 
    for sec in sections :
        half_url = "https://shopee.vn/" + sec
        output = cur_dir + "/productAndPrices.txt"
        json_file = cur_dir + "/vietnam_ward_districts_provinces.json"
        with open(output, mode="a") as appender :
            for stt in range(20) :
                url = half_url + "?page=" + str(stt)
                append_data_to_file(url, json_file, appender)
    driver.close()