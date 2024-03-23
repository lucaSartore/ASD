//Dati due interi A e B, calcolare la loro somma.

use std::collections::LinkedList;
use std::fs::File;
use std::io::{BufRead, BufReader, Write};

fn main() {
    let input = File::open("input.txt").unwrap();
    let reader = BufReader::new(input);
    let numbers: Vec<_> = reader.lines().map(
        |l| l.unwrap().split_whitespace().map(|s| s.parse::<i64>().unwrap()).collect()
    ).fold(LinkedList::<i64>::new(), |mut sum, mut x| {sum.append(&mut x); return sum;}
    ).into_iter().collect();

    let n = numbers[0]+numbers[1];

    let mut output = File::create("output.txt").unwrap();
    let _ = output.write(format!("{n}").as_bytes()).unwrap();

}