# Author: Gibson Smith
# Published: 6/11/2023

import requests
import time
from bs4 import BeautifulSoup

MAX_TIME = 0 # time limit (in minutes) will return after reached
hrefs = []





# Recursive scrape function
def scrapeWikiArticle(urlend, iterations, limit):
    # Uses BeautifulSoup to parse the Wikipedia page
    url = "https://skyrim.fandom.com" + urlend
    response = requests.get(url=url)
    soup = BeautifulSoup(response.content, 'html.parser')
    
    title = soup.find(id="firstHeading")
    table = soup.find(class_="infoboxtable")
    if table != None:
        for x in range(len(table.find_all("td"))):
            if(table.find_all("td")[x].text == "Weight"):
                print(title.text)
                print("-", table.find_all("td")[x+1].text, end="-", sep="") # these are being printed as seperate lines,
                print(table.find_all("td")[x+3].text, end="-") # easier to deal with them with ctl+f in the .csv
                print(table.find_all("td")[-1].text)
                hrefs[0]+=1
        return



    else:
        if iterations > 2 and limit == True:
            return
        # Recursively scrapes children
        if limit == True:
            if(soup.find(id="mw-pages") != None):
                allLinks = soup.find(id="mw-pages").find_all("a")
            elif(soup.find(id="mw-subcategories") != None):
                allLinks = soup.find(id="mw-subcategories").find_all("a")
            else:
                return
        else:
            allLinks = soup.find(id="mw-content-text").find_all("a")
        linkToScrape = None
        for link in allLinks:
            # Time limit check
            bench = time.time()
            elapsed = bench - startbench
            if (elapsed > (60 * MAX_TIME) and MAX_TIME > 0):
                break
            if 'href' not in link.attrs:  # Check if 'href' attribute exists
                continue
            # Prevents searches that could break the scraper
            if link['href'].find("/wiki/") == -1 or link['href'].find("https:") >= 0:
                continue
            if (link['href'].find("Help:") >= 0 or
                link['href'].find("Special:") >= 0 or link['href'].find("File:") >= 0 or
                link['href'].find("Wikipedia:") >= 0 or link['href'].find("Template:") >= 0 or
                link['href'].find("Template_talk:") >= 0 or link['href'].find("Talk:") >= 0 or
                link['href'].find("Portal:") >= 0) == True:
                continue
            linkToScrape = link
            if(linkToScrape.text == "previous page" or linkToScrape.text == "next page"):
                continue # this prevents issues with large pages like books and misc
            if linkToScrape is not None:
                if linkToScrape['href'] in hrefs:
                    if iterations != 0 or limit == True:
                        continue
                else:
                    hrefs.append(linkToScrape['href'])
                    scrapeWikiArticle(linkToScrape['href'], iterations + 1, limit)





# Main Sequence
startbench = time.time()
hrefs.append(0)
print("_______________________________________________________")
scrapeWikiArticle("/wiki/Category:Books?pagefrom=0", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Books?pagefrom=Cook%27s+Journal", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Books?pagefrom=Ildari%27s+Journal%2C+vol.+II", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Books?pagefrom=Paralysis+Poison+Recipe", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Books?pagefrom=Thief+of+Virtue", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Potions_by_level", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Scrolls_by_level", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Armor_by_level", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Clothes", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Jewelry", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Food", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Ingredients", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Tools", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Misc_items", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Misc_items", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Misc_items?pagefrom=Girl%27s+Yellow+Dress", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Misc_items?pagefrom=Storage+Room%2C+Roof", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Weapons_by_level", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Keys", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Quest_items", 0, True)
print(hrefs[0], " pages!")
scrapeWikiArticle("/wiki/Category:Items", 0, False)
print(hrefs[0], " extra pages!")

# replace "," with "", then replace "newline-" with ", "
# xx placeholders will have to be filled in manually
endbench = time.time()
print("_______________________________________________________")
elapsed = endbench - startbench
count = 0
print("Time: ", elapsed)
print("Total: ", hrefs[0])
#print("Pages: ", hrefs)