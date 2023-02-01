import json
import sys

basename_jsonfile=sys.argv[1]
basename_jsonfile_pretty=f"pretty_{basename_jsonfile}"

path_to_jsonfile = "./"+basename_jsonfile
path_to_pretty_jsonfile = "./"+basename_jsonfile_pretty

with open(path_to_jsonfile, 'r') as data_file:
    content = data_file.read()

data_json = json.loads(content)
pretty_content=json.dumps(data_json, indent=4)

with open(path_to_pretty_jsonfile, 'w') as pretty_data_file:
    pretty_data_file.write(pretty_content)
    
