from pdf_spider import PdfSpider
from scrapy.crawler import CrawlerProcess
from scrapy.utils.project import get_project_settings

msghead = '[Crawler] '

if __name__ == '__main__':
#    process = CrawlerProcess({'USER_AGENT': 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)'})
    process = CrawlerProcess( get_project_settings() )
    crawler = PdfSpider( PdfSpider.ENGINE_GOOGLE, 'keyword') # change 'keyword' to word to search
    
    print ( msghead + crawler.keyword )
    print ( msghead + crawler.engine )
    
    process.crawl(crawler)
    process.start(stop_after_crawl=True)