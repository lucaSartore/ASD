//Dati due interi A e B, calcolare la loro somma.

use std::collections::LinkedList;
use std::fs::File;
use std::io::{BufRead, BufReader, Read};

fn main() {
    let file = File::open("input.txt").unwrap();
    let reader = BufReader::new(file);
    let numbers: Vec<_> = reader.lines().map(
        |l| l.unwrap().split_whitespace().map(|s| s.parse::<i64>().unwrap()).collect()
    ).fold(Vec::<i64>::new(), |mut sum, mut x| {sum.append(&mut x); return sum;});

}