use std::collections::LinkedList;
use std::io::SeekFrom;


macro_rules! list {
    ($n:expr) => {
        Some(Box::new(ListNode::new($n)))
    };
}


fn main() {

    let mut l1: Option<Box<ListNode>> = None;
    let mut ptr_last =&mut l1;
    ptr_last = Solution::push(ptr_last, list!(2));
    ptr_last = Solution::push(ptr_last, list!(20));
    ptr_last = Solution::push(ptr_last, list!(22));
    ptr_last = Solution::push(ptr_last, list!(30));

    let mut l2: Option<Box<ListNode>> = None;
    let mut ptr_last =&mut l2;
    ptr_last = Solution::push(ptr_last, list!(1));
    ptr_last = Solution::push(ptr_last, list!(20));
    ptr_last = Solution::push(ptr_last, list!(23));
    ptr_last = Solution::push(ptr_last, list!(100));


    let mut l3: Option<Box<ListNode>> = None;
    let mut ptr_last =&mut l3;
    ptr_last = Solution::push(ptr_last, list!(10));
    ptr_last = Solution::push(ptr_last, list!(21));
    ptr_last = Solution::push(ptr_last, list!(24));
    ptr_last = Solution::push(ptr_last, list!(100));


    let final_list = Solution::merge_k_lists(vec![l1,l2,l3]);
    println!("{:?}", final_list);

}

#[derive(PartialEq, Eq, Clone, Debug)]
pub struct ListNode {
  pub val: i32,
  pub next: Option<Box<ListNode>>
}

impl ListNode {
  #[inline]
  fn new(val: i32) -> Self {
    ListNode {
      next: None,
      val
    }
  }
}

struct Solution{}

impl Solution {

    fn pop_first(list: &mut Option<Box<ListNode>>) -> Option<Box<ListNode>>{
        // list = data; owned_list = none
        let mut owned_list = Option::None;
        std::mem::swap( list, &mut owned_list);
        // list = none; owned_list = data
        let mut owned_list = owned_list?;
        std::mem::swap(&mut owned_list.next, list);
        // list = data[1..] owned_list = dat[0]
        return Some(owned_list);
    }


    fn push(ptr_last: &mut Option<Box<ListNode>>, to_push: Option<Box<ListNode>>) -> &mut Option<Box<ListNode>>{
        *ptr_last = to_push;
        let new_ptr_last = match ptr_last {
            Some(e) => e,
            None => return ptr_last
        };
        &mut new_ptr_last.next
    }

    pub fn merge_k_lists(mut lists: Vec<Option<Box<ListNode>>>) -> Option<Box<ListNode>> {
        let mut final_list: Option<Box<ListNode>> = None;
        let mut ptr_last =&mut final_list;
        loop {
            let mut min_value: &mut Option<Box<ListNode>> = &mut None;
            for v in &mut lists{
                match (&mut min_value,&v) {
                    (Some(min),Some(current)) => {
                        if current.val < min.val{
                            min_value = v;
                        }
                    }
                    (None, Some(_)) => {
                        min_value = v;
                    }
                    _ => {}
                }
            }

            let element =  Self::pop_first(min_value);

            if element.is_none(){
                break
            }

            ptr_last = Self::push(ptr_last,element);
        }
        return final_list
    }
}