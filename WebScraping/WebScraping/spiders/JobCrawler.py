import logging
import scrapy

class JobCrawler(scrapy.Spider):

    name = "JobCrawler"
    urls_to_visit = open("URLS_to_visit" , "r")
    start_urls = [url.replace("\n","") for url in urls_to_visit.readlines()]

    
    def parse(self , response):
        logger = logging.getLogger("URLS_visited")
        logger.setLevel(logging.INFO)
        logger.addHandler(logging.FileHandler("URLS_visited.log"))          
        last_page = response.xpath("//li[contains(@class , 'page last')]//a/@href").extract_first()

        for next_page in response.css(".page > a").xpath("@href").extract():
            logger.info(next_page + " has been visited")
            if next_page != last_page:
                request = scrapy.Request(response.urljoin(next_page) , callback = self.parse_item)
            else:
                request = scrapy.Request(response.urljoin(last_page) , callback = self.parse_next_page)
                request.meta['current_page'] = last_page
            yield request
            

    def parse_item(self , response):
        titles = response.xpath("//a[contains(@class,'jobLink')]/text()").extract() 
        locations = response.xpath('//span[contains(@class,"subtle")]/text()').extract() 
        companies = response.xpath("//img/@alt").extract()
        
        for item in zip(titles,locations,companies):
            scraped_info = {
                'title': item[0] , 
                'location': item[1] , 
                'company': item[2]
            }
            yield scraped_info


    def parse_next_page(self , response):
        last_page = response.xpath("//li[contains(@class , 'page last')]//a/@href").extract_first()
        current_page = response.meta['current_page']
        for i in range(0,3):
            if i == 0:
                request = scrapy.Request(response.urljoin(current_page) , callback = self.parse_item)
            else:
                request = scrapy.Request(response.urljoin(last_page) , callback = self.parse_next_page)
                request.meta['current_page'] = last_page
            yield request 

