import java.io.*;
import java.util.*;

public class Node {
  public int feature;
  public ArrayList<Node> children;
  public boolean isLeaf;
  public int label; //answer
  private double pctFeaturesSelected = 0.6;

  public Node(
      ArrayList<ArrayList<Integer>> data,
      ArrayList<Integer> features,
      boolean isRandomForest
    ) {
    this.feature = -1;
    this.train(data, features, isRandomForest);
  }

  public Node(
      ArrayList<ArrayList<Integer>> data,
      ArrayList<Integer> features,
      boolean isRandomForest,
      double pctFeaturesSelected
    ) {
    this.feature = -1;
    this.pctFeaturesSelected = pctFeaturesSelected;

    this.train(data, features, isRandomForest);
  }

  public int classify(ArrayList<Integer> data) {
  if (this.isLeaf) {
    return label;
  }
  else {
    if (data.get(this.feature) < this.children.size()
    && this.children.get(data.get(this.feature)) != null) {
    return this.children.get(data.get(this.feature)).classify(data);
    }
  }
  return label;
  }

  private void train(
      ArrayList<ArrayList<Integer>> data,
      ArrayList<Integer> features,
      boolean isRandomForest
    ) {
    this.children = new ArrayList<Node>();
    // set default label
    int l = 0;
    for (ArrayList<Integer> d : data) {
      l += d.get(0);
    }
    if (l > 0)
      label = 1;
    else
      label = -1;

    if (isStopPartitioning(data, features)) {
    int t = features.size();
    //System.out.println(data.get(0).size() + "|" + t);
      isLeaf = true;
      return;
    } else {
      isLeaf = false;
      // select feature to split
      int f = selectFeature(data, features, isRandomForest);
      if (f == -1) {
        // no more information gain
        isLeaf = true;
        return;
      }
      this.feature = f;
      // split dataset into branches
      ArrayList<Integer> new_features = new ArrayList<Integer>();
      if (isRandomForest)
        new_features = features;
      else {
        for (Integer i : features)
          if (!i.equals(f))
            new_features.add(i);
      }

      // find the max value for this feature
      int max = -1;
      for (ArrayList<Integer> d : data) {
        int i = d.get(f);
        if (i > max)
          max = i;
      }
      for (int i = 0; i <= max; i ++) {
        ArrayList<ArrayList<Integer>> new_data =
          new ArrayList<ArrayList<Integer>>();
        for (ArrayList<Integer> d : data)
          if (d.get(f) == i)
        new_data.add(d);
        if (isRandomForest)
          this.children.add(new Node(new_data, new_features, isRandomForest, this.pctFeaturesSelected));
        else
          this.children.add(new Node(new_data, new_features, isRandomForest));
      }
    }
  }

  private boolean isStopPartitioning(
      ArrayList<ArrayList<Integer>> data,
      ArrayList<Integer> features
    ) {
    if (features.size() == 0)
      return true;
    for (ArrayList<Integer> d : data) {
      if (!data.get(0).get(0).equals(d.get(0))) {
        return false;
      }
    }
    return true;
  }

  private int selectFeature(
      ArrayList<ArrayList<Integer>> data,
      ArrayList<Integer> features,
      boolean isRandomForest
    ) {
    int ans = -1;
    double max = -1;

    ArrayList<Integer> tmp_features = new ArrayList<Integer>();
    if (isRandomForest) {
      ArrayList<Integer> arr = new ArrayList<Integer>();
      for (Integer x : features) {
        arr.add(x);
      }
      Collections.shuffle(arr);
      int n = (int)(this.pctFeaturesSelected * (double)features.size());
      tmp_features.add(arr.get(0));
      for (int i = 1; i < n; i ++)
        tmp_features.add(arr.get(i));
    } else {
      tmp_features = features;
    }

    for (Integer f : tmp_features) {
      double d = gainA(data, f);
      if (d > max) {
        max = d;
        ans = f;
      }
    }

    return ans;
  }

  private double gainA(
    ArrayList<ArrayList<Integer>> data,
    int feature
  ) {
    int x = 0;
    int y = 0;
    for (ArrayList<Integer> d : data) {
      int i = d.get(0);
      if (i == -1)
      x ++;
      else
      y ++;
    }
    double infoD = entropy(x, y);
    double infoA = information(data, feature);
    return (infoD - infoA) / splitInfoA(data, feature);
  }

  private double splitInfoA(
    ArrayList<ArrayList<Integer>> data,
    int feature
  ) {
    // find the max value for this feature
    int max = -1;
    for (ArrayList<Integer> d : data) {
      int i = d.get(feature);
      if (i > max)
        max = i;
    }
    double info = 0.0;
    double size = data.size();
    int[] count = new int[max + 1];
    for (ArrayList<Integer> d : data) {
      int i = d.get(feature);
      count[i] ++;
    }
    for (int i = 0; i <= max; i ++) {
      if (count[i] > 0)
        info += -1 * ((double)count[i] / size)
          * Math.log((double)count[i] / size) / Math.log(2);
    }
    return info;
  }

  private double information(
      ArrayList<ArrayList<Integer>> data,
      int feature
    ) {
    // find the max value for this feature
    int max = -1;
    for (ArrayList<Integer> d : data) {
      int i = d.get(feature);
      if (i > max)
        max = i;
    }
    int[][] split_count = new int[max + 1][2];
    for (ArrayList<Integer> d : data) {
      int i = d.get(0);
      int j = d.get(feature);
      if (i == -1) i = 0;
      split_count[j][i] ++;
    }

    double info = 0;
    for (int i = 0; i <= max; i ++) {
      info += ((double)split_count[i][0] +
        (double)split_count[i][1]) / (double)data.size()
        * entropy(split_count[i][0], split_count[i][1]);
    }
    return info;
  }

  private double entropy(int x, int y) {
    double n_total = x + y;
    double n_yes = x;
    double n_no = y;
    double ans = 0;
    if (n_yes > 0)
      ans = -1 * (n_yes / n_total) * Math.log(n_yes / n_total) / Math.log(2);
    if (n_no > 0)
      ans += -1 * (n_no / n_total) * Math.log(n_no / n_total) / Math.log(2);
    return ans;
  }
}