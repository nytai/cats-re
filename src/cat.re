module CSSTransitionGroup = {
  external cSSTransitionGroup : ReactRe.reactClass =
    "CSSTransitionGroup" [@@bs.module "react-transition-group"];
  let createElement
      className::(className: option string)=?
      transitionName::(transitionName: string)
      transitionEnterTimeout::(transitionEnterTimeout: int)
      transitionLeaveTimeout::(transitionLeaveTimeout: int) =>
    ReactRe.wrapPropsShamelessly
      cSSTransitionGroup
      {
        /* turn an option string into a js nullable string */
        "className": Js.Null_undefined.from_opt className,
        "transitionName": Js.String.make transitionName,
        /* this stays the same, in BuckleScript and in JS */
        "transitionEnterTimeout": transitionEnterTimeout,
        "transitionLeaveTimeout": transitionLeaveTimeout
      };
};

type cat = {id: int, fact: string, image: string, isHidden: bool};

module Cat = {
  include ReactRe.Component;
  type props = {cat, byeSelf: unit => unit};
  let name = "Cat";
  let render {props} => {
    let {cat: {id, image, fact, isHidden}, byeSelf} = props;
    let catItem =
      isHidden ?
        ReactRe.nullElement :
        <li key=(string_of_int id) className="cat-item">
          <img src=image className="cat-image" />
          <p className="cat-fact"> (ReactRe.stringToElement fact) </p>
          <a
            className="delete-cat"
            href="#"
            onClick=(
              fun e => {
                ReactEventRe.Mouse.preventDefault e;
                byeSelf ()
              }
            )>
            (ReactRe.stringToElement "Bye")
          </a>
        </li>;
    <CSSTransitionGroup transitionName="cat" transitionEnterTimeout=500 transitionLeaveTimeout=500>
      catItem
    </CSSTransitionGroup>
  };
};

include ReactRe.CreateComponent Cat;

let createElement ::cat ::byeSelf => wrapProps {cat, byeSelf};
