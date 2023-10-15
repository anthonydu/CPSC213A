import static java.lang.System.out;

/**
 * A runnable class that implements and tests an ordered binary tree of integer
 * values.
 */
public class BinaryTree {

  /**
   * A node of the binary tree containing the node's integer value
   * and pointers to its right and left children (or null).
   */
  static class Node {
    int value;
    Node parent;
    Node left;
    Node right;

    /**
     * Create a new node with no children.
     */
    Node(int value) {
      this.value = value;
      this.parent = null;
      this.left = null;
      this.right = null;
    }

    /**
     * Insert the node n into the binary tree rooted by this node.
     */
    void insert(Node n) {
      if (n.value <= value) {
        if (left == null) {
          left = n;
          n.parent = this;
        } else {
          left.insert(n);
        }
      } else {
        if (right == null) {
          right = n;
          n.parent = this;
        } else {
          right.insert(n);
        }
      }
    }

    /**
     * Print the contents entire binary tree in order of ascending integer value.
     */
    void printInOrder() {
      if (left != null)
        left.printInOrder();
      out.printf("%d\n", value);
      if (right != null)
        right.printInOrder();
    }

    /**
     * Print path of values from root to specified node in orderer starting from
     * root.
     * Each node in path indicates direction taken (i.e., left or right) from parent
     * to arive at node.
     */
    void printPath() {
      if (parent != null) {
        parent.printPath();
      }
      out.printf("%s: %d\n", parent == null ? "from root" : parent.left == this ? "left to" : "right to", value);
    }

  }

  /**
   * Create a new tree populated with values provided on the command line and
   * print it in depth-first order.
   */
  public static void main(String[] args) {
    Node root = null;
    // read values from command line and add them to the tree
    Node lastNodeInserted = null;
    for (String arg : args) {
      int value = Integer.parseInt(arg);
      Node n = new Node(value);
      if (root == null)
        root = n;
      else
        root.insert(n);
      lastNodeInserted = n;
    }
    // print results
    if (root != null) {
      out.printf("In Order:\n");
      root.printInOrder();
      out.printf("Path to %d:\n", lastNodeInserted.value);
      lastNodeInserted.printPath();
    }
  }

}