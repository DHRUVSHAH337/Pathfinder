#include <iostream>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace rapidxml;

#define l1 long long int
#define l2 long double

const long double RadiusofEarth = 6378;
const long double pi=M_PI;

typedef struct points{
double lat;
double lon;
//l1 id;
}nodes;

int compare(char a,char b)
{
    if(a>=65 &&a<=90)
    {
        if(b>=65 && b<=90)
        {
            if(b==a)
                return 1;
            else
                return 0;
        }
        else
        {
            if((a+32)==b)
                return 1;
            else return 0;
        }
    }
    else
    {
        if(b>=65 && b<=90)
        {
            if(a==(b+32))
                return 1;
            else
                return 0;
        }
        else
        {
            if(a==b)
                return 1;
            else return 0;
        }
    }
}

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
    cout << "\nParsing the data (map.osm)....." << endl;

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

    for (xml_node<> * node = root_node->first_node("node"); node; node = node->next_sibling("node"))
    {
        nodes temp;
        l1 id;
        id=stoll(node->first_attribute("id")->value());
        temp.lat=stold(node->first_attribute("lat")->value());
        temp.lon=stold(node->first_attribute("lon")->value());
        v[id]=temp;
        ans++;

    }

    cout<<"Number of nodes is : "<<ans<<"\n";

    ans=0;
    for (xml_node<> * ways = root_node->first_node("way"); ways; ways = ways->next_sibling("way"))
    {
        ans++;
    }
    cout<<"Number of ways is : "<<ans<<"\n";




    ans=0;
    vector<pair<string,l1>>Nameid;

    //map<l1,nodes>searchingmap;
    for (xml_node<> * namenode = root_node->first_node("node"); namenode; namenode = namenode->next_sibling("node"))
    {
        for(xml_node<> * name_node_tag = namenode->first_node("tag"); name_node_tag; name_node_tag = name_node_tag->next_sibling("tag"))
        {
            if(name_node_tag->first_attribute("k")!=NULL)
            {
                char n[]="name";
                if(strcmp(name_node_tag->first_attribute("k")->value(),n)==0)
                {

                    pair<string,l1>temp;
                    temp.first=name_node_tag->first_attribute("v")->value();
                    temp.second=stoll(namenode->first_attribute("id")->value());
                    cout<<temp.first<<" "<<temp.second<<"\n";
                    Nameid.push_back(temp);
                    ans++;

                }
            }
        }
    }
    cout<<ans<<"\n";

    cout<<"These are the nodes having their name. Please enter a substring to search and the results would be shown to you\n\n";

    string s;
    cin>>s;

    cout.precision(12);
    //cout<<compare('t','T')<<" " <<compare('A','b')<<" "<<compare('a','b')<<"\n";

    l1 count=0;

    for(auto p:Nameid)
    {
        string check=p.first;


        //if(check=="Duan")
        int size1=check.size();
        int size2=s.size();
            //cout<<(size1-size2)<<" "<<check<<"\n";
        for(int i=0;i<=(size1-size2);i++)
        {
             //cout<<i<<"\n";

             int t=0;
            if(compare(check[i],s[0])==1)
            {
                //cout<<compare(check[i],s[0])<<" "<<check[i]<<" "<<s[0]<<" ";
                for(int j=0;j<s.size();j++)
                {
                    //cout<<compare(check[i+j],s[j])<<" "<<check[i+j]<<" "<<s[j]<<" ";
                    if(compare(check[i+j],s[j])==0)
                    {
                        t=1;
                        break;
                    }

                }
            }
            else
                t=1;

            if(t==0)
            {
                cout<<"Found the "<<++count<<" best search result \n";
                cout<<"Name of the place is "<<p.first<<"\n";
                cout<<"Its id is "<<p.second<<",Longitutde :"<<v[p.second].lon<<", Latitude :"<<v[p.second].lat<<"\n\n";
                break;
            }


        }



    }

    l1 headid,k;
    cout<<"Please enter the id for which you have to find its neighbouring points .Also tell the number of neighbouring points you have to find\n";
    cin>>headid>>k;

    vector<pair<l2,l1>>lowestkpoints;
    l2 lata=v[headid].lat;
    l2 lona=v[headid].lon;

    for(auto it:v)
    {
        if(it.first!=headid)
        {
            l2 distance=distanceusinglatlon((it.second).lat,lata,(it.second).lon,lona);
            lowestkpoints.push_back({distance,it.first});
        }
    }
    sort(lowestkpoints.begin(),lowestkpoints.end());

    for(int i=0;i<k;i++)
    {
        cout<<lowestkpoints[i].second<<" "<<lowestkpoints[i].first<<"km\n";
    }


    return 0;
}
