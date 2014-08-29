import java.io.*;
import java.util.*;

public class RandomForest {

  private ArrayList<ArrayList<Integer>> data;
  private Node[] forest;
  private final int numDecisionTree;
  private final double pctSample;
  private final double pctFeaturesSelected;

  public RandomForest(int num, double pct, double sample_pct) {
    this.numDecisionTree = num;
    this.pctFeaturesSelected = pct;
    this.pctSample = sample_pct;
    this.forest = new Node[num];
  }

  private void readTrainingFile(String fileName) {
    BufferedReader br;
    try {
      br = new BufferedReader(new FileReader(fileName));
      data = new ArrayList<ArrayList<Integer>>();
          String line = br.readLine();
          while (line != null) {
              String[] arr = line.split("\t");
              ArrayList<Integer> tmp = new ArrayList<Integer>();
            for (String str : arr) {
              if (str.equals("+1"))
                str = "1";
              tmp.add(Integer.parseInt(str));
            }
            data.add(tmp);
            line = br.readLine();
          }
          br.close();
      } catch (IOException ex) {
      }
  }

  private void trainRandomForest(String fileName) {
    this.readTrainingFile(fileName);
    ArrayList<Integer> features = new ArrayList<Integer>();
    for (int i = 1; i < data.get(0).size(); i ++) {
      features.add(i);
    }

    ArrayList<ArrayList<Integer>> new_data =
      new ArrayList<ArrayList<Integer>>();
    int sample_size = (int)((double)data.size() * this.pctSample);
    for (int i = 0; i < sample_size; i ++) {
      int x = (int)(Math.random() * data.size());
      new_data.add(data.get(x));
    }
    for (int i = 0; i < this.numDecisionTree; i ++) {
      this.forest[i] = new Node(new_data, features, true, this.pctFeaturesSelected);
    }
  }

  private void testRandomForest(String fileName) {
    BufferedReader br;
    int tp = 0;
    int fp = 0;
    int tn = 0;
    int fn = 0;
    try {
      br = new BufferedReader(new FileReader(fileName));
        String line = br.readLine();
          while (line != null) {
              String[] arr = line.split("\t");
              ArrayList<Integer> tmp = new ArrayList<Integer>();
            for (String str : arr) {
              if (str.equals("+1"))
                str = "1";
              tmp.add(Integer.parseInt(str));
            }
            int x = this.classify(tmp);
            if (tmp.get(0).equals(1)) {
              if (tmp.get(0).equals(x)) {
                tp ++;
              } else {
                fp ++;
              }
            } else {
              if (tmp.get(0).equals(x)) {
                tn ++;
              } else {
                fn ++;
              }
            }
            line = br.readLine();
          }
          br.close();
      } catch (IOException ex) {
      }

      System.out.println(tp);
      System.out.println(fn);
      System.out.println(fp);
      System.out.println(tn);
/*
      System.out.println("Accuracy: " + (double)(tp + tn)/(tp + tn + fp + fn));

      System.out.println("Error rate: " + (double)(fp + fn)/(tp + tn + fp + fn));

      System.out.println("Sensitivity: " + (double)(tp)/(tp + fn));
      System.out.println("Specificity: " + (double)(tn)/(tn + fp));
      System.out.println("Precision: " + (double)(tp)/(tp + fp));

      double percision = (double)(tp)/(tp + fp);
      double recall = (double)(tp)/(tp + fn);
      double f1 = 2 * percision * recall / (percision + recall);
      System.out.println("F-1 Score: " + f1);
*/
  }

  private int classify(ArrayList<Integer> data) {
    int nPos = 0;
    int nNeg = 0;
    for (int i = 0; i < this.numDecisionTree; i ++) {
      if (forest[i].classify(data) == 1)
        nPos ++;
      else
        nNeg ++;
    }
    if (nPos > nNeg)
      return 1;
    else
      return -1;
  }

  public static void main(String[] args) {
    RandomForest dt = new RandomForest(100, 0.6, 0.8);
    dt.trainRandomForest(args[0]);
    dt.testRandomForest(args[1]);
  }
}