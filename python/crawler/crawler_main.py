import sys
from file_crawler import FileCrawler

MSG_HEAD = '[Crawler] '
EXIT_CODE = '~'
origin_dir = '~/scraped/'

# === method definition === #
def custom_crawl():
    
    print(MSG_HEAD + 'Welcome to Console Input Mode!')    
    print(MSG_HEAD + 'Input ~ to exit at any question.')
    
    engine = input(MSG_HEAD + 'Enter engine to use(google or yahoojp): ')
    if engine is EXIT_CODE:
        sys.exit()
    elif engine is FileCrawler.ENGINE_GOOGLE:
        pass
    elif engine is FileCrawler.ENGINE_YAHOOJP:
        pass
    else:
        engine = FileCrawler.ENGINE_GOOGLE

    file_type = input(MSG_HEAD + 'Enter file type to download(pdf or ppt): ')
    if file_type is EXIT_CODE:
        sys.exit()
    elif file_type is FileCrawler.FILETYPE_PDF:
        pass
    elif file_type is FileCrawler.FILETYPE_PPT:
        pass
    else:
        file_type = FileCrawler.FILETYPE_PDF
            
    keyword = input(MSG_HEAD + 'Enter keywords to search: ')    
    if keyword is EXIT_CODE:
        sys.exit()
    
    while True:
        start_page = input(MSG_HEAD + 'Enter a page number to start the search(1 to 10): ')
        if start_page is EXIT_CODE:
            sys.exit()
        try:
            start_page = int(start_page)        
            break
        except ValueError:
            print (MSG_HEAD + 'Your input is not a number.')
    
    while True:
        end_page = input(MSG_HEAD + 'Enter a page number to end the search(1 to 10, bigger than or same as the page number to start): ')
        if end_page is EXIT_CODE:
            sys.exit()
        try:
            end_page = int(end_page)            
        except ValueError:
            print(MSG_HEAD + 'Your input is not a number.')
            
        if end_page >= start_page:
                break
        else:
            print(MSG_HEAD + 'Input a number bigger than or same as the page number to start!')

    crawler = FileCrawler(engine, file_type, keyword, start_page, end_page, origin_dir)
    crawler.crawl()
    return None
    
def preset_crawl():
    engine = FileCrawler.ENGINE_GOOGLE
    file_type = FileCrawler.FILETYPE_PDF
    keyword = 'keyword' # change 'keyword' to word to search
    start_page = 1
    end_page = 1
    
    print(MSG_HEAD + 'Welcome to Preset Mode!')    
    print(MSG_HEAD + 'It will start crawling automatically with preset options.')
    print(MSG_HEAD + 'Here are preset options:')
    print(MSG_HEAD + 'Engine: ' + engine)
    print(MSG_HEAD + 'File Type: ' + file_type)
    print(MSG_HEAD + 'Keyword: ' + keyword)
    print(MSG_HEAD + 'Start Page: ' + str(start_page))
    print(MSG_HEAD + 'End Page: ' + str(end_page))    
    
    crawler = FileCrawler(engine, file_type, keyword, start_page, end_page, origin_dir)
    crawler.crawl()
    return None

# === executiion === #
custom_crawl() # <=== change here, whether custom_crawl() or preset_crawl()
