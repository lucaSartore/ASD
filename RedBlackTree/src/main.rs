use std::borrow::BorrowMut;
use std::cell::RefCell;
use std::cmp::Ordering;
use std::fmt::Debug;
use std::rc::Rc;
use std::sync::Arc;
use crate::NodeKind::Black;

fn main() {
    println!("Hello, world!");
}

#[derive(Debug, PartialEq, Eq, Copy, Clone)]
enum NodeKind{
    Red,
    Black,
}

macro_rules! unwrap_or_return {
    ($option:expr, $return_value:expr) => {
        match $option {
            Some(x) => x,
            None => return $return_value,
        }
    };
    () => {};
}

#[derive(Debug, PartialEq, Eq)]
enum FatherPtr<T: Ord + Eq>{
    None,
    Left(Tree<T>),
    Right(Tree<T>),
}

type NodePtr<T> = Option<Rc<RefCell<Node<T>>>>;

#[derive(Debug, PartialEq, Eq)]
struct Tree<T: Ord + Eq>{
    root: NodePtr<T>
}

impl<T: Ord + Eq> Tree<T>{
    pub fn insert_node(&mut self, value: T){

        /////////////////////////// CASE 1 ///////////////////////////

        // case 1: empty tree
        if self.root.is_none(){
            self.root = Some(Rc::new(RefCell::new(Node{
                father: FatherPtr::None,
                value,
                kind: NodeKind::Black,
                left: Tree{root: None},
                right: Tree{root: None},
            })));
            return;
        }
        // the father can't be empty because we already checked that
        let father_tree = self.find_father(&value);
        let father_node = father_tree.root.clone().unwrap();

        /////////////////////////// CASE 2 ///////////////////////////

        // case 2: the father is black, so i can insert the node directly
        if father_node.borrow().kind == Black{
            let insert_to;
            let father_ptr;
            if father_node.borrow().value > value{
                // to the left
                insert_to =  &mut (*father_node).borrow_mut().right;
                father_ptr = FatherPtr::Right(father_tree.shallow_clone());
            }else{
                // to the right
                insert_to =  &mut (*father_node).borrow_mut().left;
                father_ptr = FatherPtr::Left(father_tree.shallow_clone());
            }
            // if the code is correct the insert_to must be empty here
            assert!(insert_to.root == None);
            //assert_eq!(insert_to.root, None);
            *insert_to = Tree{
                root: Some(Rc::new(RefCell::new(Node{
                father: father_ptr,
                value,
                kind: NodeKind::Red,
                left: Tree{root: None},
                right: Tree{root: None},
            })))};

        }
    }

    fn get_color(&self) -> NodeKind{
        match self.root {
            Some(ref node) => node.borrow().kind,
            None => Black,
        }
    }

    fn is_empty(&self) -> bool{
        self.root.is_none()
    }

    fn shallow_clone(&self) -> Self{
        Tree{root: self.root.clone()}
    }

    // return the note to witch this value need to be attached
    fn find_father(&self, value: &T) -> Tree<T>{
        let current = &self.root;
        // if the tree is empty, the current tree must be the father.
        let mut current = unwrap_or_return!(current,  self.shallow_clone());
        let next_tree;
        if current.borrow().value > *value{
            // to the left
            next_tree = current.borrow().left.shallow_clone();
        }else{
            // to the right
            next_tree = current.borrow().right.shallow_clone();
        }
        // if the next tree is empty it means that the current tree is the father
        if next_tree.is_empty(){
            return self.shallow_clone();
        }
        // if the next tree is not empty it means that the current tree is not the father
        // so we need to go deeper
        return next_tree.find_father(value);
    }



}

#[derive(Debug, PartialEq, Eq)]
struct Node<T: Ord + Eq> {
    father: FatherPtr<T>,
    value: T,
    kind: NodeKind,
    left: Tree<T>,
    right: Tree<T>,
}
impl<T: Ord + Eq> PartialOrd for Node<T> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
impl<T: Ord + Eq> Ord for Node<T> {
    fn cmp(&self, other: &Self) -> Ordering {
        self.value.cmp(&other.value)
    }
}