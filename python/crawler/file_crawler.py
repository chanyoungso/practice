import os
import shutil
import urllib3
import time
from bs4 import BeautifulSoup

class FileCrawler():

    # constants
    GOOGLE_BASE_URL = 'http://www.google.com/search'
    YAHOOJP_BASE_URL = ''
    FILETYPE_PDF = 'pdf'
    FILETYPE_PPT = 'ppt'
    ENGINE_GOOGLE = 'google'
    ENGINE_YAHOOJP = 'yahoojp'
    MSG_HEAD = '[PikachuCrawler] '
    
    # variables
    keyword = ''
    engine = ENGINE_GOOGLE
    file_type = FILETYPE_PDF
    start_page = 1
    end_page = 1
    origin_dir = 'D:/crawled_data/'    
    
    http = urllib3.PoolManager()    
    
    def __init__(self, engine=ENGINE_GOOGLE, file_type=FILETYPE_PDF, keyword='', start_page=1, end_page=1, origin_dir='' ):
        self.engine = engine
        self.file_type = file_type
        self.keyword = keyword
        self.start_page = start_page
        self.end_page = end_page
        self.origin_dir = origin_dir
        return None
    
    def set_url(self, engine, keyword, start_page):        
        search_url = ''
        if engine is self.ENGINE_GOOGLE:
            search_url = self.GOOGLE_BASE_URL + '?as_q=' + keyword + '&start=' + str(start_page) + '&as_filetype=' + self.FILETYPE_PDF            
        elif engine is self.ENGINE_YAHOO:
            search_url = self.YAHOOJP_BASE_URL + '?p=' + keyword + '&b=' + str(start_page) + '&vf=' + self.FILETYPE_PDF
        return search_url

    def crawl(self):
        pages = range(int(self.start_page), int(self.end_page) + 1)        
        
        for cur_page in pages:            
            print (self.MSG_HEAD + 'Crawling page No.' + str(cur_page) + ' ...' )        
            search_url = self.set_url(self.ENGINE_GOOGLE, self.keyword.replace(' ', '+'), str(cur_page))
            print (self.MSG_HEAD + 'URL is ' + search_url)
            file_url_list = self.extract_file_url(search_url)
            save_dir = self.origin_dir + self.keyword
            self.downlaod_files(save_dir, file_url_list)
            
        print(self.MSG_HEAD + 'Crawling with ' + self.keyword + ' at ' + self.engine + ', pages from ' + str(self.start_page) + ' to ' + str(self.end_page) + ' complete' )
            
        return None

    def extract_file_url(self, search_url):        
        response = self.http.request('GET', search_url)
        soup = BeautifulSoup(response.data, 'lxml')
        a_href_list = soup.select('h3.r > a') # h3.r > a::attr(href)
        file_url_list = []
        
        for a_href in a_href_list:
            file_url = str(a_href.get('href'))
            file_url = file_url[7:file_url.find('&')] # '/url?q=' in front of extracted line, '&sa=' after '.pdf'
            file_url_list.append(file_url)            
            
        response.release_conn()
        return file_url_list

    def downlaod_files(self, save_dir, file_url_list):
        
        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
            
        for file_url in file_url_list:
            response = self.http.request('GET', file_url, preload_content=False)
            time_stamp = str(time.time()).replace('.','')
            save_path = save_dir + '/' + self.engine + '_' + self.keyword + '_' + time_stamp + '.' + self.file_type
            with open(save_path, mode='wb') as file:
                shutil.copyfileobj(response, file)
                file.close()
            response.release_conn()
            print(self.MSG_HEAD + 'file downloaded: ' + save_path)
        
        return None

    pass
