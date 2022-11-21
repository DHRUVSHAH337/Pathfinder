#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace rapidxml;

#define l1 long long int
#define l2 long double
#define inf 1000000000

const long double RadiusofEarth = 6378;
const long double pi=M_PI;

typedef struct points{
double lat;
double lon;
//l1 id;
}nodes;


l2 degreetoRadians(const l2 degree)
{

    l2 unitdegree= (pi) / 180;
    return unitdegree* degree;
}

l2 distanceusinglatlon(l2 lata, l2 latb,l2 lona, l2 lonb)
{
    lata = degreetoRadians(lata),latb = degreetoRadians(latb),lona = degreetoRadians(lona),lonb = degreetoRadians(lonb);

    // Haversine Formula
    l2 dlon = abs(lonb - lona);
    l2 dlat = abs(lata - latb);

    l2 ans = pow(sin(dlat / 2), 2) + (cos(lata) * cos(latb) *pow(sin(dlon / 2), 2));

    ans = 2 * asin(sqrt(ans));
    ans = ans * RadiusofEarth;

    return ans;
}

int main(void)
{

xml_document<> doc;
xml_node<> * root_node = NULL;
    cout << "\nParsing my students data (map.xml)....." << endl;

    // Read the sample.xml file
    ifstream theFile ("map.osm");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the buffer
    doc.parse<0>(&buffer[0]);
    map<l1,nodes>v;

    // Find out the root node
    root_node = doc.first_node("osm");

    // Iterate over the student nodes
    l1 ans=0;

    set<pair<l2,l1>>minset;
    set<l1>allids;


    for (xml_node<> * node = root_node->first_node("node"); node; node = node->next_sibling("node"))
    {
        nodes temp;
        l1 id;
        id=stoll(node->first_attribute("id")->value());
        temp.lat=stold(node->first_attribute("lat")->value());
        temp.lon=stold(node->first_attribute("lon")->value());
        v[id]=temp;
         allids.insert(id);
        ans++;

    }



    cout<<"Number of nodes is : "<<ans<<"\n";
    l1 numnodes=ans;

    ans=0;

    map<l1,set<l1>>adj;
    for (xml_node<> * waynode = root_node->first_node("way"); waynode; waynode = waynode->next_sibling("way"))
    {
        xml_node<> * waysetnodes = waynode->first_node("nd");
        if(waysetnodes->first_attribute("ref")!=NULL)
        {

             l1 ini=stoll(waysetnodes->first_attribute("ref")->value());
             waysetnodes = waysetnodes->next_sibling("nd");
            for(; waysetnodes; waysetnodes = waysetnodes->next_sibling("nd"))
            {
                if(waysetnodes->first_attribute("ref")!=NULL)
                {
                    adj[ini].insert(stoll(waysetnodes->first_attribute("ref")->value()));
                    adj[stoll(waysetnodes->first_attribute("ref")->value())].insert(ini);
                    ini=stoll(waysetnodes->first_attribute("ref")->value());
                }
            }
        }

        ans++;
    }
    cout<<"Number of ways is : "<<ans<<"\n";

    cout<<"Please enter the starting point\n";
    l1 start;
    cin>>start;

    map<l1,l2>mindistance;
    //set<pair<l2,l1>>minset;
    minset.insert({0,start});

    mindistance[start]=0;

    for(auto i:allids)
    {
        if(i!=start)
            {
                mindistance[i]=inf;
                minset.insert({inf,i});
            }
    }

    while(!minset.empty())
    {
        auto temp=minset.begin();
        minset.erase(minset.begin());
        mindistance[temp->second]=temp->first;

        for(auto j:adj[temp->second])
        {
            if(mindistance[j]>mindistance[temp->second]+distanceusinglatlon(v[j].lat,v[temp->second].lat,v[j].lon,v[temp->second].lon))
            {
                minset.erase(minset.find({mindistance[j],j}));
                mindistance[j]=mindistance[temp->second]+distanceusinglatlon(v[j].lat,v[temp->second].lat,v[j].lon,v[temp->second].lon);
                minset.insert({mindistance[j],j});

            }
        }
    }




   l1 last;
   cout<<"Please enter the ending point\n";
    cin>>last;

    if(mindistance[last]<inf-1)
    cout<<mindistance[last]<<" km\n";
    else
        cout<<"Sorry there is no way conneting the start to end\n";







    ans=0;




    return 0;
}

