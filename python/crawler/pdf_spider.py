import scrapy

class PdfSpider(scrapy.Spider):

    name = 'PdfSpider'
    msghead = '[PdfSpider] '

    # constants
    GOOGLE_BASE_URL = 'http://www.google.com/search'
    YAHOOJP_BASE_URL = ''
    FILETYPE_PDF = 'pdf'
    ENGINE_GOOGLE = 'google'
    ENGINE_YAHOOJP = 'yahoojp'
    engine = ''
    keyword = ''    

    # variable declarations
    file_type = '' # a phrase necessary to search for certain file type; it depends on the website
    
    def __init__(self, engine=ENGINE_GOOGLE, keyword=''):
        self.keyword = keyword        
        self.engine = engine
        return None
    
    def set_url(self, start_page):
        if self.engine == self.ENGINE_GOOGLE:
            search_url = self.GOOGLE_BASE_URL + '?as_q=' + self.keyword + '&start=' + str(start_page) + '&as_filetype=' + self.FILETYPE_PDF
        elif self.engine == self.ENGINE_YAHOO:
            search_url = self.YAHOOJP_BASE_URL + '?as_q=' + self.keyword + '&start=' + str(start_page) + '&as_filetype=' + self.FILETYPE_PDF # to be changed
        return search_url

    def start_requests(self):
        start_page = 1 # to be changed
        search_url = self.set_url(start_page)
        print (self.msghead + 'url is ' + search_url)
        yield scrapy.Request(search_url, callback=self.parse)

    def parse(self, response):
#        href = response.css('div.r')[0].extract() # to test
#        print ( href )
        print ( self.msghead + 'Parsing complete' )
        return True