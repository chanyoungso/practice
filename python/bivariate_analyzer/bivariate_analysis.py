import analyzer
import os

unitable_file = 'unitable.xlsx'
file_list = [ unitable_file ]
bivariate_file_list = []
unitable_sheet_list = [ 'DS' ]

def bivariate_analysis( unitable_file, unitable_sheet, bivariate_file ):
    
    ua = analyzer.UnitableAnalyzer( unitable_file, bivariate_file )
    ua.set_target_sheet( unitable_sheet )
    ua.write_to_bivariate_file()
    ua.save_to_file()

    ba = analyzer.BivariateAnalyzer( bivariate_file )
    sheet_list = ba.get_sheet_name_list()

    for cur_sheet in sheet_list:    
        ba.set_target_sheet( cur_sheet )
        ba.analyze_current_sheet()
    
    ba.save_to_file()
    ba.close_file()  

for cur_item in unitable_sheet_list:
    cur_bivariate_file = 'bivariate_' + cur_item + '.xlsx'
    bivariate_file_list.append( cur_bivariate_file )
    bivariate_analysis( unitable_file, cur_item, cur_bivariate_file )    
