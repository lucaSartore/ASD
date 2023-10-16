use std::cmp::Ordering;
use std::ops::Index;
use std::vec;

#[allow(dead_code)]
mod test;

fn main() {

    let v1 = vec![0,0,0,0,0,11,12];
    let v2 = vec![20,20,20,20];

    //print!("{}",find_median_sorted_arrays(v1,v2));


    println!("\n================================================\n");

    let v1 = vec![1,2,3,4,5];
    let v2 = vec![6,7,8,9];

    print!("{}",find_median_sorted_arrays(v1,v2));
}

// avoid problems by making the two vectors start and stop from the same index
struct MyVec<'a>{
    vec: &'a Vec<i32>
}
impl<'a> MyVec<'a>{
    fn new(vec: &'a Vec<i32>) -> Self{
        Self{vec}
    }

    fn len(&self) -> usize{
        self.vec.len()+2
    }

    fn get_ptr(&self, index: usize) -> Pointer{
        Pointer{
            index,
            value: self[index]
        }
    }
}

impl Index<usize> for MyVec<'_>{
    type Output = i32;
    fn index(&self, index: usize) -> &Self::Output {
        if index == 0{
            &i32::MIN
        }
        else if index == self.vec.len()+1{
            &i32::MAX
        }else {
            & self.vec[index-1]
        }
    }
}

#[derive(PartialEq,Eq,Copy, Clone,Debug)]
struct Pointer{
    index: usize,
    value: i32,
}

impl Pointer {
    fn in_between(&self, other: Self, vector: &MyVec) -> Self{
        if (other.index as isize - self.index as isize).abs() == 1{
            return  other;
        }
        let index = (self.index+other.index)/2;
        let value = vector[index];
        Self{index,value}
    }
    /*fn in_between(left: Self,  right: Self, vector: &MyVec) -> Self{

        let index = (left.index+right.index)/2;
        let value = vector[index];
        Self{index,value}
    }*/
}

impl PartialOrd for Pointer{
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
impl Ord for Pointer {
    fn cmp(&self, other: &Self) -> Ordering {
        self.value.cmp(&other.value)
    }
}

#[derive(Eq, PartialEq)]
enum VN{V1,V2}

pub fn find_median_sorted_arrays(nums1: Vec<i32>, nums2: Vec<i32>) -> f64 {
    // ad two bounds on the left and right to make the two vectors be in the same range
    let v1 = MyVec::new(&nums1);
    let v2 = MyVec::new(&nums2);

    let mut ptr1 = v1.get_ptr(v1.len()/2);
    let mut ptr1_left = v1.get_ptr(0);
    let mut ptr1_right = v1.get_ptr(v1.len()-1);

    let mut ptr2 = v2.get_ptr(v2.len()/2);
    let mut ptr2_left = v2.get_ptr(0);
    let mut ptr2_right = v2.get_ptr(v2.len()-1);

    let target = (v1.len() + v2.len())/2;


    for i in 0..10 {
        let mut left_target = ptr1_left.index + ptr2_left.index;
        let mut right_target = v1.len() + v2.len() -2 - ptr1_right.index - ptr2_right.index;

        println!("{left_target} - {target} - {right_target}");

        // try advance left Target
        if ptr1 < ptr2 && ptr1.index + ptr2_left.index <= target{
            ptr2_left = ptr1;
            ptr1 = ptr1.in_between(ptr1_right,&v1);
        }
        // tre advance right target
        else if ptr2 < ptr1 && ptr2.index + ptr1_left.index <= target{

        }

        println!("1: {:?} - {:?} - {:?}",ptr1_left,ptr1,ptr1_right);
        println!("2: {:?} - {:?} - {:?}",ptr2_left,ptr2,ptr2_right);
    }

    return ptr1.value as f64;
}
/*
fn get_lower_bound(ptr1_left: Pointer, ptr2_left: Pointer) -> usize{
    ptr1_left.index + ptr2_left.index + 2
}

fn get_higher_bound(ptr1_right: Pointer, ptr2_right: Pointer, v1: &MyVec, v2: &MyVec) -> usize{
    v1.len() + v2.len() - ptr1_right.index - ptr2_right.index
}

fn test_lower_bound(ptr1: Pointer, ptr2: Pointer) -> usize{
    ptr1.index + ptr2.index + 2
}

fn test_higher_bound(ptr1: Pointer, ptr2: Pointer, v1: &MyVec, v2: &MyVec) -> usize{
    v1.len() + v2.len() - ptr1.index - ptr2.index
}

fn advance_left(target: usize, ptr1: Pointer, ptr2: Pointer, ptr1_left: &mut Pointer, ptr2_left: &mut Pointer) -> Result<VN,VN>{
    if test_lower_bound(ptr1,ptr2) < target{
        if ptr1 < ptr2{
            *ptr1_left = ptr1;
            Ok(VN::V1)
        }else {
            *ptr2_left = ptr2;
            Ok(VN::V2)
        }
    }else{
        if ptr1 > ptr2{
            Err(VN::V1)
        }else {
            Err(VN::V2)
        }
    }
}

fn advance_right(target: usize, ptr1: Pointer, ptr2: Pointer, ptr1_right: &mut Pointer, ptr2_right: &mut Pointer, v1: &MyVec, v2: &MyVec) -> Result<VN,VN>{
    if test_higher_bound(ptr1,ptr2,v1,v2) < target{
        if ptr1 > ptr2{
            *ptr1_right = ptr1;
            Ok(VN::V1)
        }else {
            *ptr2_right = ptr2;
            Ok(VN::V2)
        }
    }else{
        if ptr1 < ptr2{
            Err(VN::V1)
        }else {
            Err(VN::V2)
        }
    }
}*/