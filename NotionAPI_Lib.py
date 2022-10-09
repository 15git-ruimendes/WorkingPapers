from queue import Empty
import PySimpleGUI as sg
import os
import json
from PySimpleGUI.PySimpleGUI import main
from bibtexparser.bibtexexpression import strip_after_new_lines
import requests
import bibtexparser
import rispy
import nbib
from collections import namedtuple
from requests.api import get

# Altera as proximas duas linhas para o que tens ja no programa
token = 'secret_E6vzRN4OizzWsD8C6CAYHFzFrMUQAfSVFK0iWLnToAe'
database_id = 'b691e2dcb08e4266b1b4922026ee6a67'
               
lib_struct = namedtuple(
    "lib_struct", "Publisher Year Authors DOI title BibTex")

headers = {
    "Authorization": "Bearer " + token,
    "Content-Type": "application/json",
    "Notion-Version": "2021-05-13"
}


def createPage(database_id, headers, data):
    create_url = 'https://api.notion.com/v1/pages'
    for i in range(len(data)):
        new_page = {
            "parent": {"database_id": database_id},
            "properties": {
                "Title": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].title
                            }
                        }
                    ]
                },
                "Authors": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].Authors
                            }
                        }
                    ]
                },
                "Year": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].Year
                            }
                        }
                    ]
                },
                "DOI": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].DOI
                            }
                        }
                    ]
                },
                "Publisher": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].Publisher
                            }
                        }
                    ]
                },
                "BibTex": {
                    "rich_text": [
                        {
                            "text": {
                                "content": data[i].BibTex
                            }
                        }
                    ]
                }
            }
        }
        data2 = json.dumps(new_page)

        res = requests.request("POST", create_url, headers=headers, data=data2)
        print(res.status_code)


def getData(file_path):
    BibTex = ''
    if file_path.find(".bib") != -1:
        with open(file_path, encoding="utf-8") as bibtex_file:
            bibtex_str = bibtex_file.read()
        bib_database = bibtexparser.loads(bibtex_str)
        BibTex = bibtex_str
        BibTex = " "
        data = []
        print(bib_database.strings)
        for i in range(len(bib_database.entries)):

            Publisher = ""
            DOI = ""
            Year = ""
            Authors = ""
            title = ""
            if "journal" in bib_database.entries[i]:
                Publisher = bib_database.entries[i]["journal"]
            elif "booktitle" in bib_database.entries[i]:
                Publisher = bib_database.entries[i]["booktitle"]
            if "doi" in bib_database.entries[i]:
                DOI = bib_database.entries[i]["doi"]
            if "year" in bib_database.entries[i]:
                Year = bib_database.entries[i]["year"]
            if "author" in bib_database.entries[i]:
                Authors = bib_database.entries[i]["author"]
            if "title" in bib_database.entries[i]:
                title = bib_database.entries[i]["title"]

            data.append(lib_struct(Publisher, Year,
                                   Authors, DOI, title, BibTex))

        return data

    elif file_path.find(".ris") != -1:
        with open(file_path, 'r', encoding="utf-8") as bibliography_file:
            entries = rispy.load(bibliography_file)

        if "journal_name" in entries[0]:
            Publisher = entries[0]["journal_name"]
        elif "place_published" in entries[0]:
            Publisher = entries[0]["place_published"]

        if "year" in entries[0]:
            Year = entries[0]["year"]
        elif "publication_year" in entries[0]:
            Year = entries[0]["publication_year"]
        elif "date" in entries[0]:
            Year = entries[0]["date"]

        Authors_aux = entries[0]["authors"]
        Authors = ''
        for i in Authors_aux:
            Authors = Authors + i + '; '

        if "title" in entries[0]:
            title = entries[0]["title"]
        elif "primary_title" in entries[0]:
            title = entries[0]["primary_title"]

        DOI = entries[0]["doi"]
    elif file_path.find(".nbib") != -1:
        with open(file_path, 'r', encoding="utf-8") as nbib_file:
            nbib_refs = nbib.read(nbib_file.read())
        title = nbib_refs[0]["title"]
        Year = nbib_refs[0]["publication_date"]
        Publisher = nbib_refs[0]["journal_abbreviated"]
        DOI = nbib_refs[0]["doi"]
        Authors_aux = nbib_refs[0]["authors"]
        Authors = ''
        for x in Authors_aux:
            Authors = Authors + x["author"] + "; "

    else:
        f = open(file_path, "r", encoding="utf-8")
        Authors = f.readline()
        title = f.readline()
        Publisher = f.readline()
        f.readline()
        Year = f.readline()
        f.readline()
        f.readline()
        DOI = f.readline()

    data = lib_struct(Publisher, Year, Authors, DOI, title, BibTex)
    return data


def GUI():
    CONFIG_FILE = r"/home/ruimendes/Pictures/config.txt"
    f = open(CONFIG_FILE, 'r', encoding="utf-8")
    val1 = f.readline()
    val1 = val1.rstrip("\n")
    val2 = f.readline()
    val2 = val2.rstrip("\n")
    f.close()
    layout = [
        [sg.Text("File Path To Citations To Upload")],
        [sg.Input(val1), sg.FolderBrowse()],
        [sg.Text("File Path To Save Citations")],
        [sg.Input(val2), sg.FolderBrowse()],
        [sg.Button("Upload")],
        [sg.Input(), sg.FileBrowse()],
        [sg.Button("Compile Entries")]
    ]
    main_window = sg.Window(
        title="Notion app for Library Management", layout=layout)

    while True:
        event, values = main_window.read()

        if event == "Upload":
            if values[0] and values[1]:
                f = open(CONFIG_FILE, 'w', encoding="utf-8")
                f.writelines([values[0]+"\n", values[1]])
                f.close()
                UploadCit(values[0], values[1])
            else:
                val1 = val1.rstrip("\n")
                UploadCit(val1, val2)
            main_window["Upload"].update("Done")
        if event == "Compile Entries":
            GetBibTexFile(values[2])
            main_window["Compile Entries"].update("Done")
        if event == sg.WIN_CLOSED:
            break

    main_window.close()


def GetBibTexFile(FilePath):
    data = """
    {
        "filter":
            {
            "property": "UC",
            "select":{
                "equals": "Thesis"
                }
            }
	    }
    """
    read_url = 'https://api.notion.com/v1/databases/' + database_id + '/query'
    res = requests.request("POST", read_url, headers=headers, data=data)
    response = res.json()["results"]
    str_compiled = ''
    aux = ''
    title = ''
    for i in response:
        print()
        title = i["properties"]["RefName"]["title"][0]["plain_text"]
        aux = i["properties"]["BibTex"]["rich_text"][0]["text"]["content"]
        aux = aux[:aux.find('{')+1] + title + aux[aux.find(','):]
        str_compiled += aux
        str_compiled += '\n'

    f = open(FilePath, 'w')
    f.write(str_compiled)
    f.close()
    # print(str_compiled)


def UploadCit(FolderCit, FolderCitDisc):
    Files = [f for f in os.listdir(FolderCit) if os.path.isfile(
        os.path.join(FolderCit, f))]
    for x in Files:
        Path = FolderCit + "/" + x
        Path_to_Done = FolderCitDisc + "/" + x
        data = getData(Path)
        createPage(database_id, headers, data)
        os.replace(Path, Path_to_Done)

# readDatabase(database_id,headers)


GUI()
