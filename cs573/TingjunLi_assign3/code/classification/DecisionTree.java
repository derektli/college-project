import java.io.*;
import java.util.*;

public class DecisionTree {

  private ArrayList<ArrayList<Integer>> data;
  private Node root;

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

  private void trainDecisionTree(String fileName, boolean isRandomForest) {
    this.readTrainingFile(fileName);
    ArrayList<Integer> features = new ArrayList<Integer>();
    for (int i = 1; i < data.get(0).size(); i ++) {
      features.add(i);
    }
    root = new Node(data, features, isRandomForest);
  }

  private void testDecisionTree(String fileName) {
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
            int x = root.classify(tmp);
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

  public static void main(String[] args) {
    DecisionTree dt = new DecisionTree();
    dt.trainDecisionTree(args[0], false);
    dt.testDecisionTree(args[1]);
  }
}