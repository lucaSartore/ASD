use std::collections::LinkedList;
use std::fs::File;
use std::io::Read;
use std::time::{Duration, Instant};

static mut TIME_SPENT_MERGE: Duration = Duration::new(0, 0);
static mut TIME_SPENT_SPLIT: Duration = Duration::new(0, 0);

fn main() {
    // open the file
    let mut file = File::open("input.txt").unwrap();

    // put everything inside a list of numbers
    let mut contents = String::new();
    file.read_to_string(&mut contents).unwrap();
    contents = contents.replace("\n","");
    let mut numbers: LinkedList<usize> = contents
        .trim()
        .split_whitespace()
        .map(|s| s.parse().unwrap())
        .collect();

    // find out how many lines
    let number_of_lines = numbers.pop_front().unwrap_or(0);

    for i in 0..number_of_lines{
        let num_brackets = numbers.pop_front().unwrap();
        let _kind_brackets = numbers.pop_front().unwrap();

        // split the sequence of brakets
        let mut sequence = numbers;
        numbers = sequence.split_off(num_brackets);

        let start = Instant::now();
        let result = simplify(sequence,0);
        let end = Instant::now();

        let duration = end-start;
        println!("Execution time: rust {} us",duration.as_micros());
        unsafe {
            println!("\t Split is: {:2.2} % of the execution or {} us", TIME_SPENT_SPLIT.as_micros() as f64 / duration.as_micros() as f64 *100., TIME_SPENT_SPLIT.as_micros());
            println!("\t Merge is: {:2.2} % of the execution or {} us", TIME_SPENT_MERGE.as_micros() as f64 / duration.as_micros() as f64*100., TIME_SPENT_MERGE.as_micros());
        }

        if result.is_empty(){
            println!("The sequence {i} was valid!")
        }else{
            println!("The sequence {i} was invalid!")
        }
    }
}

fn merge(mut left: LinkedList<usize>,mut right: LinkedList<usize>) -> LinkedList<usize>{
    loop {
        if let (Some(left_value),Some(right_value)) = (left.back(), right.front()){
            if *left_value != *right_value{
                break
            }
        }else {
            break
        }
        left.pop_back();
        right.pop_front();
    }
    left.append(&mut right);
    left
}

const PREVIEW: bool = false;

fn simplify(sequence: LinkedList<usize>, tabs: usize) -> LinkedList<usize>{
    macro_rules! tb {
        () => {for _ in 0..tabs{print!("\t");}};
    }
    if PREVIEW{
        tb!(); println!("Input: {:?}", sequence);
    }

    let start = Instant::now();
    let mut left = sequence;
    let mut right = left.split_off(left.len()/2);
    let end = Instant::now();

    unsafe {
        TIME_SPENT_SPLIT += end-start;
    }

    if PREVIEW{
        tb!(); println!("Left: {:?}", left);
        tb!(); println!("Right: {:?}", right);
    }

    if left.len() > 1{
        left = simplify(left,tabs+1);
    }
    if right.len() > 1{
        right = simplify(right, tabs+1);
    }

    if PREVIEW{
        tb!(); println!("Left simplified: {:?}", left);
        tb!(); println!("Right simplified: {:?}", right);
    }

    let start = Instant::now();
    let merged = merge(left,right);
    let end = Instant::now();

    unsafe {
        TIME_SPENT_MERGE += end-start;
    }

    if PREVIEW{
        tb!(); println!("Merged : {:?}", merged);
    }

    merged
}