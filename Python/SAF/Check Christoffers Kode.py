#Check Christoffers Kode
import xml.etree.ElementTree as ET
import csv
import socket
import time

xml_file_path = ""
csv_file_path = ""
station_name = "Station#06"

xml = '<?xml version="1.0" encoding="UFT-8"><info><time>'+5+'</time><ID>'+10+'</ID></info>'

def parse_xml_data(root):
        # Parse the XML data and return as a string
        id_element = root.find('ID')
        if id_element is not None:
            id_value = id_element.text
            print('ID: %s' % id_value)
        else:
            print('ID element not found in the XML string.')

        return id_value

def search_csv(csv_file, column_name, row_keyword):
    with open(csv_file, 'r') as file:
        csv_reader = csv.reader(file)
        headers = next(csv_reader)  # Read the header row
        try:
            col_index = headers.index(column_name)
            matching_rows = []
            for row in csv_reader:
                if row_keyword in row:
                    matching_rows.append(row[col_index])
            return matching_rows
        except ValueError:
            return None





tree = ET.parse(xml_file_path)
root = tree.getroot()
# Parse the XML data here and publish the results as a ROS2 message
id_value = parse_xml_data(root)

# Search for the ID in the CSV file
results = search_csv(csv_file_path, station_name, id_value)
if results is not None:
    print('CSV Row Data:')
    for result in results:
        print(result)
else:
    print('ID not found in the CSV file.')