import json
from urllib.request import urlopen

dataset_id = "belib-points-de-recharge-pour-vehicules-electriques-disponibilite-temps-reel"

domain = "https://parisdata.opendatasoft.com"
starting_path = "/api/v2"

query = "timezone=UTC"

req = f"{domain}{starting_path}/catalog/datasets/{dataset_id}/records?{query}"

content = urlopen(req)
data_json = json.loads(content.read())

print(req)
# print(json.dumps(data_json, indent=4))