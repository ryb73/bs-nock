open Jest;
open Expect;
open PromEx;

describe("Nock", () => {
    open Nock;

    test("cleanAll", () => {
        nock("http://lol.com")
        |> get("/")
        |> reply(200);

        Nock.cleanAll();

        expect(Nock.isDone())
        |> toBe(true)
    });

    describe("isDone", () => {
        testPromise("true", () => {
            nock("http://lol.com")
            |> get("/")
            |> reply(200);

            Superagent.get("http://lol.com/")
            |> Superagent.end_
            |> map(_ => Nock.isDone())
            |> map(expect)
            |> map(toBe(true))
        });

        test("false", () => {
            nock("http://lol.com")
            |> get("/")
            |> reply(200);

            expect(Nock.isDone())
            |> toBe(false)
        });
    });

    testPromise("get", () => {
        nock("http://lol.com")
        |> get("/")
        |> reply(200);

        Superagent.(
            Superagent.get("http://lol.com/")
            |> end_
            |> map(({ statusCode }) => statusCode)
            |> map(expect)
            |> map(toBe(200))
        )
    });

    testPromise("post", () => {
        nock("http://lol.com")
        |> post("/")
        |> reply(200);

        Superagent.(
            Superagent.post("http://lol.com/")
            |> end_
            |> map(({ statusCode }) => statusCode)
            |> map(expect)
            |> map(toBe(200))
        )
    });

    describe("reply", () => {
        testPromise("empty", () => {
            nock("http://lol.com")
            |> post("/")
            |> reply(200);

            Superagent.(
                Superagent.post("http://lol.com/")
                |> end_
                |> map(({ text }) => text)
                |> map(expect)
                |> map(toBe(""))
            )
        });

        let jsonTest = (name, json) =>
            testPromise(name, () => {
                nock("http://lol.com")
                |> post("/")
                |> reply(~response=json, 200);

                Superagent.(
                    Superagent.post("http://lol.com/")
                    |> end_
                    |> map(({ text }) => text)
                    |> map(Js.Json.parseExn)
                    |> map(expect)
                    |> map(toEqual(json))
                )
            });

        jsonTest("string", Js.Json.string("string"));
        jsonTest("number", Js.Json.number(3.));
        jsonTest("true", Js.Json.boolean(true));
        jsonTest("false", Js.Json.boolean(false));
        jsonTest("null", Js.Json.null);

        [|("x",Js.Json.string("1"))|]
        |> Js.Dict.fromArray
        |> Js.Json.object_
        |> jsonTest("object");

        [|1., 2., 3.|]
        |> Js.Array.map(Js.Json.number)
        |> Js.Json.array
        |> jsonTest("array");
    });

    testPromise("replyObj", () => {
        nock("http://lol.com")
        |> post("/")
        |> replyObj(200, {"a":"b"});

        Superagent.(
            Superagent.post("http://lol.com/")
            |> end_
            |> map(({ text }) => text)
            |> map(expect)
            |> map(toBe("{\"a\":\"b\"}"))
        )
    });
});
