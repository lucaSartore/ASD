
fn main() {
    let n = 5;
    let relations = vec![vec![1,5],vec![2,5],vec![3,5],vec![3,4],vec![4,5]];
    let time = vec![1,2,3,4,5];

    let result = Solution::minimum_time(n, relations, time);
    println!("{result}");
}
struct Solution;


use std::cell::RefCell;

struct Course<'a>{
    requirements: RefCell<Vec<&'a Course<'a>>>,
    cost_with_req: RefCell<Option<i32>>,
    cost: i32
}

impl<'a> Course<'a>{
    fn new(cost: i32) -> Self{
        Course{
            requirements: RefCell::new(Vec::new()),
            cost_with_req: RefCell::new(None),
            cost
        }
    }

    fn add_requirement(&'a self, requirement: &'a Course<'a>){
        self.requirements.borrow_mut().push(requirement);
    }

    fn get_cost(&self) -> i32{
        if let Some(c) = *self.cost_with_req.borrow(){
            return c;
        }

        let mut cost = 0;
        for req in self.requirements.borrow().iter(){
            cost = cost.max(req.get_cost());
        }

        let final_cost = cost+self.cost;
        *self.cost_with_req.borrow_mut() = Some(final_cost);
        return final_cost;
    }
}

impl Solution {
    pub fn minimum_time(n: i32, relations: Vec<Vec<i32>>, time: Vec<i32>) -> i32 {
        let mut courses = Vec::with_capacity(n as usize);

        for cost in time{
            courses.push(Course::new(cost));
        }

        for relation in relations.iter(){
            let required_course = relation[0] as usize - 1;
            let course_with_requirement = relation[1] as usize - 1;
            courses[course_with_requirement].add_requirement(&courses[required_course]);
        }

        let mut max_cost = 0;
        for course in &courses{
            max_cost = max_cost.max(course.get_cost());
        }

        max_cost
    }
}

/*
    pub fn minimum_time(_: i32, relations: Vec<Vec<i32>>, time: Vec<i32>) -> i32 {

        let courses: Vec<Course> = time.iter().map(|cost| Course::new(*cost)).collect();

        relations.iter().for_each(|r|{
            let required_course = r[0] as usize - 1;
            let course_with_requirement = r[1] as usize - 1;
            courses[course_with_requirement].add_requirement(&courses[required_course]);
        });

        courses.iter().fold(0, |acc, course| acc.max(course.get_cost()))
    }
*/