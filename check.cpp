#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

const int product_type_length=16;            //16= "product_type":" (length)
const int price_length=9;                     //9= "price":"
const int grams_length=8;                     // 9="grams":
const int one_handed_kg=100000;              //100kg

int max_under_100=0;
double price_for_max_under_100;              //result price for max amount

int summation(vector<pair<int,double> > numbers){
  int sum=0;
  for(int i=0;i<numbers.size();i++){
    sum=sum+numbers[i].first;
  }
  return sum;
}
///calculate the max amount we can buy and the price.
void subset_sum(vector<pair<int,double> > numbers, vector<pair<int,double> > partial){
      int sum=summation(partial);
      if(sum>max_under_100 && sum<=one_handed_kg){                  // max amount under 100kg
          max_under_100=sum;
          price_for_max_under_100=0;                               //reset price_for_max_under_100
          for(int i=0;i<partial.size();i++){                       //calculate the price
              price_for_max_under_100+=partial[i].second;
          }
      }
      if(sum>one_handed_kg) return;                               //if over 100kg, no need to keep going

      for(int i=0;i<numbers.size();i++){
          pair<int,double> temp=numbers[i];
          numbers.erase(numbers.begin()+i);
          partial.push_back(temp);
          subset_sum(numbers,partial);
      }
}



int main(){
  string input;
  ifstream infile;
  infile.open("shoptify.txt");                                 //convert the URL content to a txt file.

  getline(infile,input);

  vector<pair<int,double> > table;

  size_t product_type_loc=0;
  while(input.find("\"product_type\":",product_type_loc+1,14)!=string::npos){
      product_type_loc=input.find("\"product_type\":",product_type_loc+1,14);       //location of "product_type"
      size_t tag_loc=input.find("\"tags\":",product_type_loc+1,7);                  //location of "tags"
      string product_type=input.substr(product_type_loc+product_type_length,tag_loc-product_type_loc-product_type_length-2); // the string between product_type and tags are what we want // why minus 2 , 1 for , 1 for "

      if(product_type=="Computer" || product_type=="Keyboard"){                   //check whether the product_type is computer or keyboard
          size_t variants_loc,images_loc;
          variants_loc=input.find("\"variants\":",tag_loc+1,11);                       //location of "variants"
          images_loc=input.find("\"images\":",variants_loc+1,9);                        // location of "images"
          string variants=input.substr(variants_loc,images_loc-variants_loc-1);        //the string between "variants" and "images"

          size_t price_loc=0;
          while(variants.find("\"price\":",price_loc+1,8)!=string::npos){
                price_loc=variants.find("\"price\":",price_loc+1,8);                   //location of "price"
                size_t gram_loc=variants.find("\"grams\":",price_loc+1,8);              //location of "grams"
                size_t compare_at_price_loc=variants.find("\"compare_at_price\":",gram_loc+1,19);      //location of "compare_at_price" (used to get string price )
                string price_s=variants.substr(price_loc+price_length,gram_loc-price_loc-price_length-2);           //get the string price
                string grams_s=variants.substr(gram_loc+grams_length,compare_at_price_loc-gram_loc-grams_length-1); //get the string grams
                double price_d=stod (price_s);                                      //string  -> double
                int grams_i=stoi(grams_s);                                          //string -> integer

                table.push_back(make_pair(grams_i,price_d));              //store pair(gram, price) into the table
          }
        }
  }
  infile.close();


  vector<pair<int,double> > partial;
  subset_sum(table,partial);
  cout<<"The max amount of computers and keyboards we can carry below 100kg is "<<max_under_100<<" grams,and the price is "<<price_for_max_under_100<<endl;
  return 0;
}
